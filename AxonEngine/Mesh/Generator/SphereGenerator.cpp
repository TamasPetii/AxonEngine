#include "SphereGenerator.h"
#include <glm/gtc/constants.hpp>
#include <cmath>

namespace Axn 
{
    SphereGenerator::SphereGenerator(float radius, uint32_t sectors, uint32_t stacks)
        : _radius(radius), _sectors(sectors), _stacks(stacks) {}

    MeshBuildData SphereGenerator::Generate() const 
    {
        MeshBuildData data;

        for (uint32_t stack = 0; stack <= _stacks; ++stack) 
        {
            float v = static_cast<float>(stack) / _stacks;
            float phi = v * glm::pi<float>();

            for (uint32_t sector = 0; sector <= _sectors; ++sector) 
            {
                float u = static_cast<float>(sector) / _sectors;
                float theta = u * 2.0f * glm::pi<float>();

                float x = _radius * std::sin(phi) * std::cos(theta);
                float y = _radius * std::cos(phi);
                float z = _radius * std::sin(phi) * std::sin(theta);

                glm::vec3 pos(x, y, z);
                glm::vec3 normal = (glm::length(pos) > 0.0001f) ? glm::normalize(pos) : glm::vec3(0.0f, 1.0f, 0.0f);

                data.vertexData.positions.push_back({ pos, 0 });
                data.vertexData.attributes.push_back({ normal, u, glm::vec3(0.0f), v });
            }
        }

        for (uint32_t stack = 0; stack < _stacks; ++stack) 
        {
            for (uint32_t sector = 0; sector < _sectors; ++sector) 
            {
                uint32_t current = stack * (_sectors + 1) + sector;
                uint32_t next = current + (_sectors + 1);

                data.vertexData.indices.push_back(current);
                data.vertexData.indices.push_back(current + 1);
                data.vertexData.indices.push_back(next);

                data.vertexData.indices.push_back(current + 1);
                data.vertexData.indices.push_back(next + 1);
                data.vertexData.indices.push_back(next);
            }
        }

        data.subMeshes.push_back({
            0,
            static_cast<uint32_t>(data.vertexData.indices.size()),
            0
        });

        return data;
    }
}