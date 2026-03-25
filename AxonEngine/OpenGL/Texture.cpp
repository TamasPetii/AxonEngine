#include "Texture.h"

namespace Axn 
{
    Texture::Texture(const TextureSpec& spec) : _spec(spec) 
    {
        glCreateTextures(_spec.target, 1, &_id);

        if (_spec.target == GL_TEXTURE_CUBE_MAP || _spec.target == GL_TEXTURE_2D_ARRAY) 
        {
            glTextureStorage3D(_id, 1, _spec.internal_format, _spec.width, _spec.height, _spec.layers);
        }
        else 
        {
            glTextureStorage2D(_id, 1, _spec.internal_format, _spec.width, _spec.height);
        }

        glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, _spec.min_filter);
        glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, _spec.mag_filter);
        glTextureParameteri(_id, GL_TEXTURE_WRAP_S, _spec.wrap_s);
        glTextureParameteri(_id, GL_TEXTURE_WRAP_T, _spec.wrap_t);
        glTextureParameteri(_id, GL_TEXTURE_WRAP_R, _spec.wrap_r);

        if (_spec.bindless) 
        {
            _handle = glGetTextureHandleARB(_id);
        }
    }

    Texture::~Texture() 
    {
        if (_handle != 0) 
            MakeNonResident();

        glDeleteTextures(1, &_id);
    }

    void Texture::SetData(const void* data) 
    {
        glTextureSubImage2D(_id, 0, 0, 0, _spec.width, _spec.height, _spec.format, _spec.type, data);
        if (_spec.generate_mipmaps) glGenerateTextureMipmap(_id);
    }

    void Texture::SetDataLayer(const void* data, uint32_t layer) 
    {
        glTextureSubImage3D(_id, 0, 0, 0, layer, _spec.width, _spec.height, 1, _spec.format, _spec.type, data);
        if (_spec.generate_mipmaps) glGenerateTextureMipmap(_id);
    }

    void Texture::MakeResident() 
    {
        if (_handle != 0 && !glIsTextureHandleResidentARB(_handle))
            glMakeTextureHandleResidentARB(_handle);
    }

    void Texture::MakeNonResident() 
    {
        if (_handle != 0 && glIsTextureHandleResidentARB(_handle))
            glMakeTextureHandleNonResidentARB(_handle);
    }
}