#include "Framebuffer.h"
#include <stdexcept>

namespace Axn 
{
    Framebuffer::Framebuffer(uint32_t width, uint32_t height) 
        : _width(width), _height(height) 
    {
        glCreateFramebuffers(1, &_id);
    }

    Framebuffer::~Framebuffer() 
    {
        glDeleteFramebuffers(1, &_id);
    }

    void Framebuffer::Bind() const 
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glViewport(0, 0, _width, _height);
    }

    void Framebuffer::UnBind() const 
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::AddAttachment(const std::string& name, const TextureSpec& spec, GLenum attachment_type) 
    {
        _attachment_specs[name] = { spec, attachment_type };
        
        TextureSpec actual_spec = spec;
        actual_spec.width = _width;
        actual_spec.height = _height;
        
        auto tex = std::make_shared<Texture>(actual_spec);
        _textures[name] = tex;

        glNamedFramebufferTexture(_id, attachment_type, tex->GetID(), 0);

        if (attachment_type >= GL_COLOR_ATTACHMENT0 && attachment_type <= GL_COLOR_ATTACHMENT15) 
        {
            _draw_buffers.push_back(attachment_type);
            glNamedFramebufferDrawBuffers(_id, (GLsizei)_draw_buffers.size(), _draw_buffers.data());
        }
    }

    void Framebuffer::Resize(uint32_t width, uint32_t height) 
    {
        _width = width;
        _height = height;

        glDeleteFramebuffers(1, &_id);
        glCreateFramebuffers(1, &_id);

        _textures.clear();
        _draw_buffers.clear();

        auto specs_to_rebuild = _attachment_specs;
        _attachment_specs.clear();

        for (const auto& [name, info] : specs_to_rebuild) 
        {
            AddAttachment(name, info.spec, info.type);
        }
        
        CheckStatus();
    }

    void Framebuffer::Clear() 
    {
        static const float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        static const float clear_depth = 1.0f;

        for (int i = 0; i < (int)_draw_buffers.size(); ++i)
        {
            glClearNamedFramebufferfv(_id, GL_COLOR, i, clear_color);
        }
        
        glClearNamedFramebufferfv(_id, GL_DEPTH, 0, &clear_depth);
    }

    void Framebuffer::CheckStatus() 
    {
        if (glCheckNamedFramebufferStatus(_id, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("Framebuffer is not complete!");
        }
    }

    std::shared_ptr<Texture> Framebuffer::GetTexture(const std::string& name) 
    {
        return _textures.at(name);
    }
}