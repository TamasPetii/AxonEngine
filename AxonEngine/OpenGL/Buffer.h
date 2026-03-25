#pragma once
#include <GL/glew.h>
#include <span>

namespace Axn 
{
    enum class BufferUsage : GLbitfield 
    {
        Static  = 0,
        Dynamic = GL_DYNAMIC_STORAGE_BIT,
        MapRead  = GL_MAP_READ_BIT,
        MapWrite = GL_MAP_WRITE_BIT,
        Persistent = GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT
    };

    class Buffer 
    {
    public:
        Buffer();
        ~Buffer();

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        template<typename T>
        void Allocate(std::span<const T> data, BufferUsage usage = BufferUsage::Static);
        
        void* Map(BufferUsage access, GLintptr offset = 0, GLsizeiptr length = -1);
        void Unmap();

        void BindAsSsbo(GLuint binding_point) const;
        void BindAsIndexBuffer() const;
        
        GLuint GetID() const noexcept { return _id; }
    private:
        GLuint _id = 0;
        GLsizeiptr _size = 0;
        void* _mapped_ptr = nullptr;
    };

    template<typename T>
    inline void Buffer::Allocate(std::span<const T> data, BufferUsage usage) 
    {
        _size = data.size_bytes();
        glNamedBufferStorage(_id, _size, data.data(), static_cast<GLbitfield>(usage));
    }
}