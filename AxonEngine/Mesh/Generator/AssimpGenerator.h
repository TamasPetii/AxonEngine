#pragma once
#include "../IMeshGenerator.h"
#include <string>
#include <glm/glm.hpp>

struct aiNode;
struct aiScene;

namespace Axn 
{
    class AssimpGenerator : public IMeshGenerator 
    {
    public:
        AssimpGenerator(std::string path, MaterialLoadCallback materialLoader = nullptr);
        MeshBuildData Generate() const override;

    private:
        void ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform, const std::string& directory, MeshBuildData& data, uint32_t& vertexOffset, uint32_t& indexOffset) const;
        
        std::string _path;
        MaterialLoadCallback _materialLoader;
    };
}