#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "../OpenGL/Texture.h"

namespace Axn 
{
    class TextureManager 
    {
    public:
        std::shared_ptr<Texture> LoadTexture(const std::string& path);
        std::shared_ptr<Texture> GetTexture(const std::string& path) const;
    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
    };
}