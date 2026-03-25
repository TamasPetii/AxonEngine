#pragma once
#include <memory>
#include "Mesh/StaticMesh.h"

namespace Axn 
{
    struct ModelComponent 
    {
        std::shared_ptr<StaticMesh> mesh;
        bool isVisible = true;
        bool castShadow = true;
    };
}