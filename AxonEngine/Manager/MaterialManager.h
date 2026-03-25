#pragma once
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include "../Mesh/Material.h"
#include "../OpenGL/Buffer.h"

namespace Axn 
{
    using TextureLoadCallback = std::function<GLuint64(const std::string&)>;

    class MaterialManager 
    {
    public:
        explicit MaterialManager(TextureLoadCallback textureLoader);

        uint32_t RegisterMaterial(const MaterialInfo& info);
        void SyncToGpu(uint32_t bindingPoint);
    private:
        TextureLoadCallback _textureLoader;

        bool _isDirty = false;
        std::unique_ptr<Buffer> _ssbo;
        std::vector<GpuMaterialData> _gpuMaterials;
        std::unordered_map<std::string, uint32_t> _materialMap;
    };
}