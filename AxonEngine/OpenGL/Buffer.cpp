#include "Buffer.h"

namespace Axn 
{
    Buffer::Buffer() { 
        glCreateBuffers(1, &_id);
     }

    Buffer::~Buffer() { 
        if (_id)
            glDeleteBuffers(1, &_id);
    }

    void* Buffer::Map(BufferUsage access, GLintptr offset, GLsizeiptr length) 
    {
        GLsizeiptr size = (length == -1) ? _size : length;
        _mapped_ptr = glMapNamedBufferRange(_id, offset, size, static_cast<GLbitfield>(access));
        return _mapped_ptr;
    }

    void Buffer::Unmap() 
    {
        glUnmapNamedBuffer(_id);
        _mapped_ptr = nullptr;
    }

    void Buffer::BindAsSsbo(GLuint binding_point) const 
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, _id);
    }

    void Buffer::BindAsIndexBuffer() const 
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    }
}