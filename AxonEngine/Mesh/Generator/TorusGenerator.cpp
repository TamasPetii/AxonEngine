#include "TorusGenerator.h"
#include <glm/gtc/constants.hpp>
#include <cmath>

namespace Axn 
{
    TorusGenerator::TorusGenerator(float mainRadius, float tubeRadius, uint32_t mainSegments, uint32_t tubeSegments)
        : _mainRadius(mainRadius), _tubeRadius(tubeRadius), _mainSegments(mainSegments), _tubeSegments(tubeSegments) {}

    MeshBuildData TorusGenerator::Generate() const 
    {
        MeshBuildData data;

        for (uint32_t i = 0; i <= _mainSegments; ++i) 
        {
            float u = static_cast<float>(i) / _mainSegments;
            float theta = u * 2.0f * glm::pi<float>();

            float cosTheta = std::cos(theta);
            float sinTheta = std::sin(theta);
            glm::vec3 tubeCenter(_mainRadius * cosTheta, 0.0f, _mainRadius * sinTheta);

            for (uint32_t j = 0; j <= _tubeSegments; ++j) 
            {
                float v = static_cast<float>(j) / _tubeSegments;
                float phi = v * 2.0f * glm::pi<float>();

                float cosPhi = std::cos(phi);
                float sinPhi = std::sin(phi);

                float x = (_mainRadius + _tubeRadius * cosPhi) * cosTheta;
                float y = _tubeRadius * sinPhi;
                float z = (_mainRadius + _tubeRadius * cosPhi) * sinTheta;

                glm::vec3 pos(x, y, z);
                glm::vec3 normal = glm::normalize(pos - tubeCenter);

                data.vertexData.positions.push_back({ pos, 0 });
                data.vertexData.attributes.push_back({ normal, u, glm::vec3(0.0f), v });
            }
        }

        for (uint32_t i = 0; i < _mainSegments; ++i) 
        {
            for (uint32_t j = 0; j < _tubeSegments; ++j) 
            {
                uint32_t current = i * (_tubeSegments + 1) + j;
                uint32_t next = current + (_tubeSegments + 1);

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