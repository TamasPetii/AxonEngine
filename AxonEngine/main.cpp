#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <entt/entt.hpp>
#include <assimp/Importer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "OpenGL/Buffer.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Program.h"
#include "OpenGL/Vertex.h"
#include "Manager/ShaderManager.h"

Axn::GpuVertexData GenerateCube() 
{
    Axn::GpuVertexData cube;
    
    const glm::vec3 pos[8] = {
        {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
        {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f} 
    };

    const glm::vec3 normals[6] = {
        { 0,  0,  1}, { 1,  0,  0}, { 0,  0, -1}, {-1,  0,  0}, { 0,  1,  0}, { 0, -1,  0}
    };

    int faces[6][5] = {
        {0, 1, 2, 3, 0},
        {1, 5, 6, 2, 1},
        {5, 4, 7, 6, 2},
        {4, 0, 3, 7, 3},
        {3, 2, 6, 7, 4},
        {4, 5, 1, 0, 5}
    };

    uint32_t vertexCount = 0;
    for (int i = 0; i < 6; i++) 
    {
        for (int j = 0; j < 4; j++) 
        {
            cube.positions.push_back({ pos[faces[i][j]], 0 });
            cube.attributes.push_back({ normals[faces[i][4]], 0.0f, glm::vec3(0), 0.0f });
        }

        cube.indices.insert(cube.indices.end(), { vertexCount, vertexCount + 1, vertexCount + 2, vertexCount, vertexCount + 2, vertexCount + 3 });
        vertexCount += 4;
    }

    return cube;
}

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

        Axn::GpuVertexData myCube = GenerateCube();
        Axn::Buffer posBuffer; posBuffer.Allocate<Axn::GpuVertexPosition>(myCube.positions);
        Axn::Buffer attrBuffer; attrBuffer.Allocate<Axn::GpuVertexAttributes>(myCube.attributes);
        Axn::Buffer indexBuffer; indexBuffer.Allocate<uint32_t>(myCube.indices);
        
        GLuint dummyVAO = 0;
        glCreateVertexArrays(1, &dummyVAO);
        glBindVertexArray(dummyVAO);

        Axn::ShaderManager shaderManager;
        shaderManager.LoadShader("ShaderVert", GL_VERTEX_SHADER, "Shaders/shader.vert");
        shaderManager.LoadShader("ShaderFrag", GL_FRAGMENT_SHADER, "Shaders/shader.frag");
        Axn::Program* shaderProgram = shaderManager.CreateProgram("CubeMat", {"ShaderVert", "ShaderFrag"});

        glEnable(GL_DEPTH_TEST);

        float rotationAngle = 0.0f;
        glm::vec3 cubeColor(0.2f, 0.6f, 1.0f);
        
        while (!glfwWindowShouldClose(window)) 
        {
            glfwPollEvents();

            glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            rotationAngle += 0.01f;
            glm::mat4 model = glm::mat4(1.0f); 
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::rotate(model, rotationAngle, glm::vec3(0.5f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            glm::mat4 modelIT = glm::transpose(glm::inverse(model));
            
            glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            
            int w, h;
            glfwGetFramebufferSize(window, &w, &h);
            float aspect = (float)w / (float)(h == 0 ? 1 : h);
            glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

            shaderProgram->Bind();
            shaderProgram->SetUniform("model", model);
            shaderProgram->SetUniform("modelIT", modelIT);
            shaderProgram->SetUniform("view", view);
            shaderProgram->SetUniform("proj", proj);
            shaderProgram->SetUniform("u_Color", cubeColor);

            posBuffer.BindAsSsbo(0); 
            attrBuffer.BindAsSsbo(1);
            indexBuffer.BindAsIndexBuffer();
            glDrawElements(GL_TRIANGLES, myCube.indices.size(), GL_UNSIGNED_INT, nullptr);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("AxonEngine Muszerfal");
            ImGui::Text("Modern C++20 | DSA | Vertex Pulling");
            ImGui::ColorEdit3("Kocka Szine", &cubeColor.x);
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
        // Ha bármilyen hiba történik (pl. nem találja a shadert), ide ugrik!
        std::cerr << "\n----------------------------------------" << std::endl;
        std::cerr << "[VEGZETES HIBA]: " << e.what() << std::endl;
        std::cerr << "----------------------------------------\n" << std::endl;
        
        // Ez megállítja a konzolt, hogy el tudd olvasni a hibát, mielőtt bezáródik az ablak
        system("pause"); 
        return -1;
    }
}