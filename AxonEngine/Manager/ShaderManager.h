#pragma once
#include "../OpenGL/Shader.h"
#include "../OpenGL/Program.h"
#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>
#include <initializer_list>

namespace Axn 
{
    class ShaderManager 
    {
    public:
        ShaderManager() = default;
        ~ShaderManager() = default;

        ShaderManager(const ShaderManager&) = delete;
        ShaderManager& operator=(const ShaderManager&) = delete;

        Shader* LoadShader(std::string_view name, GLenum type, std::string_view path);
        Shader* GetShader(std::string_view name) const;

        Program* CreateProgram(std::string_view name, std::initializer_list<std::string_view> shader_names);
        Program* GetProgram(std::string_view name) const;

        void Clear();
    private:
        std::unordered_map<std::string, std::unique_ptr<Shader>> _shaders;
        std::unordered_map<std::string, std::unique_ptr<Program>> _programs;
    };
}