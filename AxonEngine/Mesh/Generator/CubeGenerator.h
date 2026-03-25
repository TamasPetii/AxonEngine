#pragma once
#include "../IMeshGenerator.h"

namespace Axn 
{
    class CubeGenerator : public IMeshGenerator 
    {
    public:
        CubeGenerator(float size = 1.0f);
        MeshBuildData Generate() const override;

    private:
        float _size;
    };
}