#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "Vertex.h"
#include "Material.h"

namespace Axn 
{
    using MaterialLoadCallback = std::function<uint32_t(const MaterialInfo&)>;

    struct SubMesh 
    {
        uint32_t indexOffset;
        uint32_t indexCount;
        uint32_t materialIndex;
    };

    struct MeshBuildData 
    {
        GpuVertexData vertexData;
        std::vector<SubMesh> subMeshes;
    };

    class IMeshGenerator 
    {
    public:
        virtual ~IMeshGenerator() = default;
        virtual MeshBuildData Generate() const = 0;
    };
}