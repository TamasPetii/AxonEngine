#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Scene.h"

namespace Axn
{
    class App {
    public:
        App();
        ~App();

        void Run();
    private:
        bool InitWindow();
        void InitImGui();
        void Shutdown();

        GLFWwindow* _window = nullptr;
        std::unique_ptr<Scene> _activeScene;
    };
}

