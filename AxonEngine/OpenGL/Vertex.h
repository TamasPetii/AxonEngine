#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace Axn 
{
    struct GpuVertexPosition 
    {
        glm::vec3 position;
        uint32_t packedIndex;

        constexpr void SetMeshAndNodeIndex(uint16_t meshIndex, uint16_t nodeIndex) noexcept 
        {
            packedIndex = (static_cast<uint32_t>(nodeIndex) << 16) | meshIndex;
        }
    };

    struct GpuVertexAttributes 
    {
        glm::vec3 normal;
        float uv_x;
        glm::vec3 tangent;
        float uv_y;
    };

    struct GpuVertexData 
    {
        std::vector<uint32_t> indices;
        std::vector<GpuVertexPosition> positions;
        std::vector<GpuVertexAttributes> attributes;
    };
}