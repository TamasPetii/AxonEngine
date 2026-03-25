#pragma once
#include <GL/glew.h>
#include <string_view>

namespace Axn 
{
    class Shader 
    {
    public:
        Shader(GLenum type, std::string_view path);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        GLuint GetID() const noexcept { return _id; }
    private:
        GLuint _id = 0;
    };
}