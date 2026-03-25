#include <iostream>
#include <vector>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "OpenGL/Buffer.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Program.h"
#include "Mesh/StaticMesh.h"

#include "Manager/ShaderManager.h"
#include "Manager/ModelManager.h"
#include "Manager/TextureManager.h"
#include "Manager/MaterialManager.h"

int main() 
{
    try 
    {
        std::cout << "AxonEngine inditasa..." << std::endl;

        if (!glfwInit()) return -1;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(1280, 720, "AxonEngine", nullptr, nullptr);
        if (!window) 
        {
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) 
            return -1;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsDark();
        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::GetStyle().WindowRounding = 0.0f;
            ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460 core");

        auto texManager = std::make_unique<Axn::TextureManager>();
        
        Axn::TextureManager* texPtr = texManager.get();
        auto materialManager = std::make_unique<Axn::MaterialManager>(
            [texPtr](const std::string& path) -> GLuint64 {
                auto tex = texPtr->LoadTexture(path);
                return tex ? tex->GetHandle() : 0;
            }
        );

        Axn::MaterialManager* matPtr = materialManager.get();
        auto modelManager = std::make_unique<Axn::ModelManager>(
            [matPtr](const Axn::MaterialInfo& info) -> uint32_t {
                return matPtr->RegisterMaterial(info);
            }
        );

        auto shaderManager = std::make_unique<Axn::ShaderManager>();
        shaderManager->LoadShader("ShaderVert", GL_VERTEX_SHADER, "Shaders/shader.vert");
        shaderManager->LoadShader("ShaderFrag", GL_FRAGMENT_SHADER, "Shaders/shader.frag");
        Axn::Program* shaderProgram = shaderManager->CreateProgram("MainShader", {"ShaderVert", "ShaderFrag"});

        auto myModel = modelManager->LoadModel("C:/Users/User/Desktop/Models/Sponza-master/sponza.obj");

        GLuint dummyVAO = 0;
        glCreateVertexArrays(1, &dummyVAO);
        glBindVertexArray(dummyVAO);

        glEnable(GL_DEPTH_TEST);

        glm::vec3 camPos(0.0f, 2.0f, 0.0f);
        glm::vec3 camTarget(10.0f, 2.0f, 0.0f); 
        float modelScale = 0.01f;
        
        while (!glfwWindowShouldClose(window)) 
        {
            glfwPollEvents();

            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            materialManager->SyncToGpu(2);

            glm::mat4 model = glm::mat4(1.0f); 
            model = glm::scale(model, glm::vec3(modelScale));
            glm::mat4 modelIT = glm::transpose(glm::inverse(model));
            
            glm::mat4 view = glm::lookAt(camPos, camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
            
            int w, h;
            glfwGetFramebufferSize(window, &w, &h);
            float aspect = (float)w / (float)(h == 0 ? 1 : h);
            glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

            shaderProgram->Bind();
            shaderProgram->SetUniform("model", model);
            shaderProgram->SetUniform("modelIT", modelIT);
            shaderProgram->SetUniform("view", view);
            shaderProgram->SetUniform("proj", proj);
            
            myModel->Draw(shaderProgram);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("AxonEngine Muszerfal");
            ImGui::Text("Sponza Explorer");
            ImGui::Separator();
            ImGui::DragFloat3("Kamera Pozicio", &camPos.x, 0.1f);
            ImGui::DragFloat3("Kamera Celpont", &camTarget.x, 0.1f);
            ImGui::DragFloat("Modell Meret", &modelScale, 0.001f, 0.0001f, 1.0f);
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                GLFWwindow* backup = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup);
            }

            glfwSwapBuffers(window);
        }

        glDeleteVertexArrays(1, &dummyVAO);
        
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();

        return 0;

    }
    catch (const std::exception& e) 
    {
        std::cerr << "\n----------------------------------------" << std::endl;
        std::cerr << "[VEGZETES HIBA]: " << e.what() << std::endl;
        std::cerr << "----------------------------------------\n" << std::endl;
        system("pause"); 
        return -1;
    }
}