#pragma once
#include <string>
#include <unordered_map>
#include <memory>

#include "../Mesh/StaticMesh.h"
#include "../Mesh/Generator/AssimpGenerator.h"
#include "../Mesh/Generator/CubeGenerator.h"
#include "../Mesh/Generator/SphereGenerator.h"
#include "../Mesh/Generator/TorusGenerator.h"

namespace Axn 
{
    class ModelManager 
    {
    public:
        static constexpr const char* Cube = "Cube";
        static constexpr const char* Sphere = "Sphere";
        static constexpr const char* Torus = "Torus";

        ModelManager(MaterialLoadCallback materialLoader = nullptr);

        std::shared_ptr<StaticMesh> LoadModel(const std::string& path);
        std::shared_ptr<StaticMesh> GetModel(const std::string& nameOrPath) const;
    private:
        MaterialLoadCallback _materialLoader;
        std::unordered_map<std::string, std::shared_ptr<StaticMesh>> _models;
    };
}