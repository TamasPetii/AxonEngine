#pragma once
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Axn 
{
    struct MaterialInfo 
    {
        std::string name;
        std::string albedoPath;
        std::string normalPath;
        glm::vec4 baseColor{1.0f, 1.0f, 1.0f, 1.0f};
    };

    struct GpuMaterialData 
    {
        glm::vec4 baseColor;
        GLuint64 albedoHandle = 0;
        GLuint64 normalHandle = 0;
    };

    static_assert(sizeof(GpuMaterialData) == 32);
}