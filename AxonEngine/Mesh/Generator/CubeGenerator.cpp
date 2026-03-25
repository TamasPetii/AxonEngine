#include "CubeGenerator.h"
#include <vector>

namespace Axn 
{
    CubeGenerator::CubeGenerator(float size) : _size(size) {}

    MeshBuildData CubeGenerator::Generate() const 
    {
        MeshBuildData data;
        float h = _size * 0.5f;

        const glm::vec3 positions[24] = {
            {-h, -h,  h}, { h, -h,  h}, { h,  h,  h}, {-h,  h,  h},
            { h, -h, -h}, {-h, -h, -h}, {-h,  h, -h}, { h,  h, -h},
            {-h, -h, -h}, {-h, -h,  h}, {-h,  h,  h}, {-h,  h, -h},
            { h, -h,  h}, { h, -h, -h}, { h,  h, -h}, { h,  h,  h},
            {-h,  h,  h}, { h,  h,  h}, { h,  h, -h}, {-h,  h, -h},
            {-h, -h, -h}, { h, -h, -h}, { h, -h,  h}, {-h, -h,  h}
        };

        const glm::vec2 uvs[4] = {
            {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}
        };

        const glm::vec3 normals[6] = {
            { 0.0f,  0.0f,  1.0f},
            { 0.0f,  0.0f, -1.0f},
            {-1.0f,  0.0f,  0.0f},
            { 1.0f,  0.0f,  0.0f},
            { 0.0f,  1.0f,  0.0f},
            { 0.0f, -1.0f,  0.0f}
        };

        data.vertexData.positions.reserve(24);
        data.vertexData.attributes.reserve(24);
        data.vertexData.indices.reserve(36);

        for (int i = 0; i < 6; ++i) 
        {
            for (int j = 0; j < 4; ++j) 
            {
                data.vertexData.positions.push_back({ positions[i * 4 + j], 0 });
                data.vertexData.attributes.push_back({ normals[i], uvs[j].x, glm::vec3(0.0f), uvs[j].y });
            }

            uint32_t offset = i * 4;
            data.vertexData.indices.push_back(offset + 0);
            data.vertexData.indices.push_back(offset + 1);
            data.vertexData.indices.push_back(offset + 2);
            data.vertexData.indices.push_back(offset + 2);
            data.vertexData.indices.push_back(offset + 3);
            data.vertexData.indices.push_back(offset + 0);
        }

        data.subMeshes.push_back({
            0,
            static_cast<uint32_t>(data.vertexData.indices.size()),
            0
        });

        return data;
    }
}