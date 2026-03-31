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
#include "Manager/InputManager.h"
#include "Utils/Camera.h"

namespace Axn {
    class Scene {
    public:
        Scene(std::shared_ptr<InputManager> input);
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
        std::unique_ptr<TextureManager> _textureManager;
        std::unique_ptr<MaterialManager> _materialManager;
        std::unique_ptr<ModelManager> _modelManager;
        std::unique_ptr<ShaderManager> _shaderManager;
        std::shared_ptr<InputManager> _inputManager;
    private:
        glm::vec2 _viewportSize{ 0, 0 };
        bool _isViewportHovered = false;
        std::unique_ptr<Camera> _camera;
        
        entt::registry _registry;
        std::unique_ptr<Framebuffer> _viewportFramebuffer;      
        Program* _mainShader = nullptr;
    };
}

