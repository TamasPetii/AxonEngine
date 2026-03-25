#include "TextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

namespace Axn 
{
    std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string& path) 
    {
        if (_textures.find(path) != _textures.end()) 
        {
            return _textures[path];
        }

        int width, height, channels;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
        
        if (!data) 
        {
            std::cout << "Couldn't Load Texture: " << path << std::endl;
            return nullptr;
        }

        TextureSpec spec{};
        spec.target = GL_TEXTURE_2D;
        spec.width = width;
        spec.height = height;
        spec.layers = 1;
        spec.format = GL_RGBA;
        spec.internal_format = GL_RGBA8;
        spec.type = GL_UNSIGNED_BYTE;
        spec.min_filter = GL_LINEAR_MIPMAP_LINEAR;
        spec.mag_filter = GL_LINEAR;
        spec.wrap_s = GL_REPEAT;
        spec.wrap_t = GL_REPEAT;
        spec.wrap_r = GL_REPEAT;
        spec.bindless = true;
        spec.generate_mipmaps = true;

        auto texture = std::make_shared<Texture>(spec);
        texture->SetData(data);
        texture->MakeResident();

        std::cout << "Loaded Texture: " << path << std::endl;

        stbi_image_free(data);

        _textures[path] = texture;
        return texture;
    }

    std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& path) const 
    {
        auto it = _textures.find(path);
        if (it != _textures.end()) 
        {
            return it->second;
        }
        return nullptr;
    }
}