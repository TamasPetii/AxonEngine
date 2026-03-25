#include "ModelManager.h"

namespace Axn 
{
    ModelManager::ModelManager(MaterialLoadCallback materialLoader) 
        : _materialLoader(std::move(materialLoader))
    {
        _models[std::string(Cube)] = std::make_shared<StaticMesh>(std::make_unique<CubeGenerator>());
        _models[std::string(Sphere)] = std::make_shared<StaticMesh>(std::make_unique<SphereGenerator>());
        _models[std::string(Torus)] = std::make_shared<StaticMesh>(std::make_unique<TorusGenerator>());
    }

    std::shared_ptr<StaticMesh> ModelManager::LoadModel(const std::string& path) 
    {
        if (_models.find(path) == _models.end()) 
        {
            _models[path] = std::make_shared<StaticMesh>(std::make_unique<AssimpGenerator>(path, _materialLoader));
        }
        return _models[path];
    }

    std::shared_ptr<StaticMesh> ModelManager::GetModel(const std::string& nameOrPath) const
    {
        auto it = _models.find(nameOrPath);
        if (it != _models.end()) 
        {
            return it->second;
        }
        return nullptr;
    }
}