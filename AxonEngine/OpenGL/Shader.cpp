#include "Shader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace Axn 
{
    Shader::Shader(GLenum type, std::string_view path) 
    {
        std::ifstream file(path.data());
        if (!file.is_open()) 
            throw std::runtime_error("Failed to open shader file!");

        std::stringstream stream;
        stream << file.rdbuf();
        std::string source = stream.str();
        const char* src_ptr = source.c_str();

        _id = glCreateShader(type);
        glShaderSource(_id, 1, &src_ptr, nullptr);
        glCompileShader(_id);

        GLint success;
        glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
        if (!success) 
        {
            GLint log_length;
            glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &log_length);
            std::vector<char> log(log_length);
            glGetShaderInfoLog(_id, log_length, nullptr, log.data());
            throw std::runtime_error(log.data());
        }
    }

    Shader::~Shader() { glDeleteShader(_id); }
}