#pragma once
#include "Texture.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <GL/glew.h>

namespace Axn 
{
    struct AttachmentInfo 
    {
        TextureSpec spec;
        GLenum type;
    };

    class Framebuffer 
    {
    public:
        Framebuffer(uint32_t width, uint32_t height);
        ~Framebuffer();

        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;

        void Bind() const;
        void UnBind() const;
        void Resize(uint32_t width, uint32_t height);

        void AddAttachment(const std::string& name, const TextureSpec& spec, GLenum attachment_type);
        
        void Clear();
        void CheckStatus();

        std::shared_ptr<Texture> GetTexture(const std::string& name);
        GLuint GetID() const noexcept { return _id; }

    private:
        void Invalidate();

        GLuint _id = 0;
        uint32_t _width, _height;
        std::vector<GLenum> _draw_buffers;
        std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
        std::unordered_map<std::string, AttachmentInfo> _attachment_specs;
    };
}