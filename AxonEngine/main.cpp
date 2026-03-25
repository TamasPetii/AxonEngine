#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <entt/entt.hpp>
#include <assimp/Importer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main() {
    std::cout << "AxonEngine inditasa..." << std::endl;


    if (!glfwInit()) {
        std::cerr << "Hiba: GLFW inicializalas sikertelen!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "AxonEngine - Vcpkg Teszt", nullptr, nullptr);
    if (!window) {
        std::cerr << "Hiba: Ablak letrehozasa sikertelen!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Hiba: GLEW inicializalas sikertelen!" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    glm::vec3 testVec(1.0f, 2.0f, 3.0f);
    std::cout << "GLM teszt sikeres: Y pozicio: " << testVec.y << std::endl;

    entt::registry registry;
    auto entity = registry.create();
    std::cout << "EnTT teszt sikeres: Entitas letrehozva id: " << (uint32_t)entity << std::endl;

    Assimp::Importer importer;
    std::cout << "Assimp teszt sikeres: Importer peldanyositva." << std::endl;

    int width, height, channels;
    unsigned char* data = stbi_load("nem_letezo_kep.png", &width, &height, &channels, 0);
    if (!data) {
        std::cout << "STB Image teszt sikeres: fuggveny meghivva (kep persze nincs)." << std::endl;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("AxonEngine Debug");
        ImGui::Text("Minden vcpkg csomag tokeletesen mukodik!");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}