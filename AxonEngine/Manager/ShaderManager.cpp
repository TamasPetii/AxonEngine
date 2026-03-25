#include "ShaderManager.h"
#include <stdexcept>
#include <vector>

namespace Axn 
{
    Shader* ShaderManager::LoadShader(std::string_view name, GLenum type, std::string_view path) 
    {
        std::string key{name};
        
        if (_shaders.find(key) != _shaders.end()) 
        {
            return _shaders[key].get();
        }

        auto shader = std::make_unique<Shader>(type, path);
        Shader* raw_ptr = shader.get();
        
        _shaders[key] = std::move(shader);
        return raw_ptr;
    }

    Shader* ShaderManager::GetShader(std::string_view name) const 
    {
        std::string key{name};
        auto it = _shaders.find(key);
        
        if (it != _shaders.end()) 
            return it->second.get();
            
        return nullptr;
    }

    Program* ShaderManager::CreateProgram(std::string_view name, std::initializer_list<std::string_view> shader_names) 
    {
        std::string key{name};
        
        if (_programs.find(key) != _programs.end()) 
        {
            return _programs[key].get();
        }

        std::vector<Shader*> shader_ptrs;
        shader_ptrs.reserve(shader_names.size());

        for (auto s_name : shader_names) 
        {
            Shader* s = GetShader(s_name);
            if (!s) 
            {
                throw std::runtime_error("Hiba: Shader nem talalhato a program letrehozasakor -> " + std::string(s_name));
            }
            shader_ptrs.push_back(s);
        }

        auto program = std::make_unique<Program>(shader_ptrs);
        Program* raw_ptr = program.get();
        
        _programs[key] = std::move(program);
        return raw_ptr;
    }

    Program* ShaderManager::GetProgram(std::string_view name) const 
    {
        std::string key{name};
        auto it = _programs.find(key);
        
        if (it != _programs.end()) 
            return it->second.get();
            
        return nullptr;
    }

    void ShaderManager::Clear() 
    {
        _programs.clear();
        _shaders.clear();
    }
}