#pragma once
#include "../IMeshGenerator.h"

namespace Axn 
{
    class TorusGenerator : public IMeshGenerator 
    {
    public:
        TorusGenerator(float mainRadius = 1.0f, float tubeRadius = 0.3f, uint32_t mainSegments = 36, uint32_t tubeSegments = 18);
        MeshBuildData Generate() const override;

    private:
        float _mainRadius;
        float _tubeRadius;
        uint32_t _mainSegments;
        uint32_t _tubeSegments;
    };
}