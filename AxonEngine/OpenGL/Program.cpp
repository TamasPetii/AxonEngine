#include "Program.h"
#include "Shader.h"
#include <stdexcept>
#include <vector>

namespace Axn 
{
    Program::Program(std::span<Shader* const> shaders) 
    {
        _id = glCreateProgram();

        for (auto* shader : shaders) 
        {
            if (shader) 
            {
                glAttachShader(_id, shader->GetID());
            }
        }

        glLinkProgram(_id);

        GLint success;
        glGetProgramiv(_id, GL_LINK_STATUS, &success);
        if (!success) 
        {
            GLint log_length;
            glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &log_length);
            std::vector<char> log(log_length);
            glGetProgramInfoLog(_id, log_length, nullptr, log.data());
            throw std::runtime_error("Program Linking Error: " + std::string(log.data()));
        }

        glValidateProgram(_id);
    }

    Program::~Program() 
    {
        if (_id != 0) 
        {
            glDeleteProgram(_id);
        }
    }

    void Program::Bind() const 
    {
        glUseProgram(_id);
    }

    void Program::UnBind() const 
    {
        glUseProgram(0);
    }

    void Program::SetHandle(std::string_view name, GLuint64 handle) 
    {
        glProgramUniformHandleui64ARB(_id, GetLocation(name), handle);
    }

    GLint Program::GetLocation(std::string_view name) 
    {
        std::string key(name);
        
        if (_locations.find(key) == _locations.end()) 
            _locations[key] = glGetUniformLocation(_id, key.c_str());

        return _locations[key];
    }
}