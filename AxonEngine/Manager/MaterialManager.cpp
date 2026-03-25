#include "MaterialManager.h"
#include <iostream>

namespace Axn 
{
    MaterialManager::MaterialManager(TextureLoadCallback textureLoader)
        : _textureLoader(std::move(textureLoader)) 
    {
        _ssbo = std::make_unique<Buffer>();

        GpuMaterialData defaultMat{};
        defaultMat.baseColor = glm::vec4(1.0f);
        _gpuMaterials.push_back(defaultMat);
        _isDirty = true;
    }

    uint32_t MaterialManager::RegisterMaterial(const MaterialInfo& info) 
    {
        std::string key = info.name;
        if (_materialMap.contains(key)) 
        {
            return _materialMap[key];
        }

        GpuMaterialData gpuData{};
        gpuData.baseColor = info.baseColor;

        if (_textureLoader) 
        {
            if (!info.albedoPath.empty()) 
                gpuData.albedoHandle = _textureLoader(info.albedoPath);
            
            if (!info.normalPath.empty()) 
                gpuData.normalHandle = _textureLoader(info.normalPath);
        }

        uint32_t index = static_cast<uint32_t>(_gpuMaterials.size());
        _gpuMaterials.push_back(gpuData);
        _materialMap[key] = index;
        _isDirty = true;

        return index;
    }

    void MaterialManager::SyncToGpu(uint32_t bindingPoint) 
    {
        if (!_ssbo) return;

        if (_isDirty && !_gpuMaterials.empty()) 
        {
            _ssbo->Allocate<GpuMaterialData>(_gpuMaterials, BufferUsage::Dynamic);
            _isDirty = false;
        }

        _ssbo->BindAsSsbo(bindingPoint);
    }
}