#pragma once
#include "IMeshGenerator.h"
#include "../OpenGL/Buffer.h"
#include "../OpenGL/Program.h"
#include <memory>
#include <vector>

namespace Axn 
{
    class StaticMesh 
    {
    public:
        StaticMesh(std::unique_ptr<IMeshGenerator> generator);
        void Draw(Program* shader) const;
    private:
        std::unique_ptr<Buffer> _posBuffer;
        std::unique_ptr<Buffer> _attrBuffer;
        std::unique_ptr<Buffer> _indexBuffer;  
        std::vector<SubMesh> _subMeshes;
    };
}