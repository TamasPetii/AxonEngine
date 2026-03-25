#pragma once
#include <GL/glew.h>
#include <cstdint>

namespace Axn 
{
    struct TextureSpec 
    {
        uint32_t width = 1024;
        uint32_t height = 1024;
        uint32_t layers = 1;
        
        GLenum target = GL_TEXTURE_2D; 
        
        GLenum internal_format = GL_RGBA8;
        GLenum format = GL_RGBA;
        GLenum type = GL_UNSIGNED_BYTE;
        
        GLenum min_filter = GL_LINEAR;
        GLenum mag_filter = GL_LINEAR;
        GLenum wrap_s = GL_CLAMP_TO_EDGE;
        GLenum wrap_t = GL_CLAMP_TO_EDGE;
        GLenum wrap_r = GL_CLAMP_TO_EDGE;

        bool generate_mipmaps = false;
        bool bindless = true;
    };

    class Texture 
    {
    public:
        Texture(const TextureSpec& spec);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        void SetData(const void* data);
        void SetDataLayer(const void* data, uint32_t layer);

        void MakeResident();
        void MakeNonResident();

        GLuint GetID() const noexcept { return _id; }
        GLuint64 GetHandle() const noexcept { return _handle; }

    private:
        GLuint _id = 0;
        GLuint64 _handle = 0;
        TextureSpec _spec;
    };
}