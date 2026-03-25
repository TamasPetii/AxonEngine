#include "AssimpGenerator.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Axn 
{
    static glm::mat4 ConvertMatrix(const aiMatrix4x4& aiMat) 
    {
        return glm::mat4(
            aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
            aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
            aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
            aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
        );
    }

    AssimpGenerator::AssimpGenerator(std::string path, MaterialLoadCallback materialLoader) 
            : _path(std::move(path)), _materialLoader(std::move(materialLoader)) {}

    MeshBuildData AssimpGenerator::Generate() const 
    {
        MeshBuildData data;
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(_path, 
            aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            return data;
        }

        uint32_t vertexOffset = 0;
        uint32_t indexOffset = 0;

        std::string directory = _path.substr(0, _path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene, glm::mat4(1.0f), directory, data, vertexOffset, indexOffset);

        return data;
    }

   void AssimpGenerator::ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform, const std::string& directory, MeshBuildData& data, uint32_t& vertexOffset, uint32_t& indexOffset) const
    {
        glm::mat4 localTransform = ConvertMatrix(node->mTransformation);
        glm::mat4 globalTransform = parentTransform * localTransform;
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(globalTransform)));

        for (uint32_t i = 0; i < node->mNumMeshes; ++i) 
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

            for (uint32_t v = 0; v < mesh->mNumVertices; ++v) 
            {
                GpuVertexPosition pos{};
                glm::vec4 transformedPos = globalTransform * glm::vec4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.0f);
                pos.position = glm::vec3(transformedPos);
                pos.packedIndex = 0;

                GpuVertexAttributes attr{};
                if (mesh->HasNormals()) 
                {
                    glm::vec3 n(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
                    attr.normal = glm::normalize(normalMatrix * n);
                }
                else 
                {
                    attr.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                }

                if (mesh->HasTangentsAndBitangents()) 
                {
                    glm::vec3 t(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);
                    attr.tangent = glm::normalize(normalMatrix * t);
                }
                else 
                {
                    attr.tangent = glm::vec3(0.0f);
                }

                if (mesh->HasTextureCoords(0)) 
                {
                    attr.uv_x = mesh->mTextureCoords[0][v].x;
                    attr.uv_y = mesh->mTextureCoords[0][v].y;
                }
                else 
                {
                    attr.uv_x = 0.0f;
                    attr.uv_y = 0.0f;
                }

                data.vertexData.positions.push_back(pos);
                data.vertexData.attributes.push_back(attr);
            }

            uint32_t indexCount = 0;
            for (uint32_t f = 0; f < mesh->mNumFaces; ++f) 
            {
                const aiFace& face = mesh->mFaces[f];
                for (uint32_t j = 0; j < face.mNumIndices; ++j) 
                {
                    data.vertexData.indices.push_back(face.mIndices[j] + vertexOffset);
                    indexCount++;
                }
            }

            uint32_t currentMaterialIndex = 0;      
            if (_materialLoader && mesh->mMaterialIndex >= 0) 
            {
                aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
                MaterialInfo matInfo{};
                matInfo.name = aiMat->GetName().C_Str();

                aiColor4D color;
                if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color)) 
                {
                    matInfo.baseColor = glm::vec4(color.r, color.g, color.b, color.a);
                }

                aiString texPath;
                if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
                {
                    aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
                    matInfo.albedoPath = directory + "/" + texPath.C_Str();
                }

                if (aiMat->GetTextureCount(aiTextureType_NORMALS) > 0) 
                {
                    aiMat->GetTexture(aiTextureType_NORMALS, 0, &texPath);
                    matInfo.normalPath = directory + "/" + texPath.C_Str();
                }

                currentMaterialIndex = _materialLoader(matInfo);
            }

            data.subMeshes.push_back({
                indexOffset,
                indexCount,
                currentMaterialIndex
            });

            vertexOffset += mesh->mNumVertices;
            indexOffset += indexCount;
        }

        for (uint32_t i = 0; i < node->mNumChildren; ++i) 
        {
            ProcessNode(node->mChildren[i], scene, globalTransform, directory, data, vertexOffset, indexOffset);
        }
    }
}