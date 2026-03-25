#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include <entt/entt.hpp>
#include "OpenGL/Program.h"
#include "OpenGL/Framebuffer.h"
#include "Manager/ShaderManager.h"
#include "Manager/ModelManager.h"
#include "Manager/TextureManager.h"
#include "Manager/MaterialManager.h"

namespace Axn {
    class Scene {
    public:
        Scene();
        virtual ~Scene();

        void OnUpdate(float dt);
        void OnRender();
    private:
        void InitManagers();
        void InitShaders();
        void InitRegistry();
        void InitFramebuffer();
    private:
        GLuint _rootVAO = 0;
        std::unique_ptr<TextureManager> _texManager;
        std::unique_ptr<MaterialManager> _matManager;
        std::unique_ptr<ModelManager> _modelManager;
        std::unique_ptr<ShaderManager> _shaderManager;
    private:
        glm::vec2 _viewportSize{ 0, 0 };
        std::unique_ptr<Framebuffer> _viewportFramebuffer;
    
        entt::registry _registry;
        
        Program* _mainShader = nullptr;

        glm::vec3 _camPos{ 0.0f, 2.0f, 0.0f };
        glm::vec3 _camTarget{ 10.0f, 2.0f, 0.0f };
    };
}

