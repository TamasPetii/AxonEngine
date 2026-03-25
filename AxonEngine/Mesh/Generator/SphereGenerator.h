#pragma once
#include "../IMeshGenerator.h"

namespace Axn 
{
    class SphereGenerator : public IMeshGenerator 
    {
    public:
        SphereGenerator(float radius = 1.0f, uint32_t sectors = 36, uint32_t stacks = 18);
        MeshBuildData Generate() const override;

    private:
        float _radius;
        uint32_t _sectors;
        uint32_t _stacks;
    };
}