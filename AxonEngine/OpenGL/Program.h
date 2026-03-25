#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string_view>
#include <unordered_map>
#include <span>
#include <string>

namespace Axn 
{
    class Shader;

    class Program 
    {
    public:
        Program(std::span<Shader* const> shaders);
        ~Program();

        Program(const Program&) = delete;
        Program& operator=(const Program&) = delete;

        void Bind() const;
        void UnBind() const;

        template<typename T>
        void SetUniform(std::string_view name, const T& value);   
        void SetHandle(std::string_view name, GLuint64 handle);
        GLuint GetID() const noexcept { return _id; }
    private:
        GLint GetLocation(std::string_view name);

        GLuint _id = 0;
        std::unordered_map<std::string, GLint> _locations;
    };

    template<typename T>
    inline void Program::SetUniform(std::string_view name, const T& value) 
    {
        static_assert(sizeof(T) == 0, "Unsupported Uniform Type!");
    }

    template<>
    inline void Program::SetUniform<bool>(std::string_view name, const bool& value) 
    {
        glProgramUniform1i(_id, GetLocation(name), static_cast<int>(value));
    }

    template<>
    inline void Program::SetUniform<int>(std::string_view name, const int& value) 
    {
        glProgramUniform1i(_id, GetLocation(name), value);
    }

    template<>
    inline void Program::SetUniform<uint32_t>(std::string_view name, const uint32_t& value) 
    {
        glProgramUniform1ui(_id, GetLocation(name), value);
    }

    template<>
    inline void Program::SetUniform<float>(std::string_view name, const float& value) 
    {
        glProgramUniform1f(_id, GetLocation(name), value);
    }

    template<>
    inline void Program::SetUniform<glm::vec2>(std::string_view name, const glm::vec2& value) 
    {
        glProgramUniform2f(_id, GetLocation(name), value.x, value.y);
    }

    template<>
    inline void Program::SetUniform<glm::vec3>(std::string_view name, const glm::vec3& value) 
    {
        glProgramUniform3f(_id, GetLocation(name), value.x, value.y, value.z);
    }

    template<>
    inline void Program::SetUniform<glm::vec4>(std::string_view name, const glm::vec4& value) 
    {
        glProgramUniform4f(_id, GetLocation(name), value.x, value.y, value.z, value.w);
    }

    template<>
    inline void Program::SetUniform<glm::mat3>(std::string_view name, const glm::mat3& value) 
    {
        glProgramUniformMatrix3fv(_id, GetLocation(name), 1, GL_FALSE, &value[0][0]);
    }

    template<>
    inline void Program::SetUniform<glm::mat4>(std::string_view name, const glm::mat4& value) 
    {
        glProgramUniformMatrix4fv(_id, GetLocation(name), 1, GL_FALSE, &value[0][0]);
    }
}