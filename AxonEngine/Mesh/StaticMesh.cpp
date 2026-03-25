#include "StaticMesh.h"

namespace Axn 
{
    StaticMesh::StaticMesh(std::unique_ptr<IMeshGenerator> generator) 
    {
        MeshBuildData data = generator->Generate();
        _subMeshes = std::move(data.subMeshes);

        _posBuffer = std::make_unique<Buffer>();
        _attrBuffer = std::make_unique<Buffer>();
        _indexBuffer = std::make_unique<Buffer>();

        _posBuffer->Allocate<GpuVertexPosition>(data.vertexData.positions);
        _attrBuffer->Allocate<GpuVertexAttributes>(data.vertexData.attributes);
        _indexBuffer->Allocate<uint32_t>(data.vertexData.indices);
    }

    void StaticMesh::Draw(Program* shader) const 
    {
        if (!_posBuffer || !_attrBuffer || !_indexBuffer) return;

        _posBuffer->BindAsSsbo(0);
        _attrBuffer->BindAsSsbo(1);
        _indexBuffer->BindAsIndexBuffer();

        for (const auto& subMesh : _subMeshes) 
        {
            shader->SetUniform("u_MaterialIndex", subMesh.materialIndex);        
            glDrawElements(GL_TRIANGLES, subMesh.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(subMesh.indexOffset * sizeof(uint32_t)));
        }
    }
}