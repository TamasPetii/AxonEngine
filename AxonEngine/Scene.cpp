#include "Scene.h"
#include <GL/glew.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Component/TransformComponent.h"
#include "Component/ModelComponent.h"
#include <iostream>
#include "Utils.h"

namespace Axn
{
    Scene::Scene() {
        InitManagers();
        InitShaders();
        InitRegistry();
        InitFramebuffer();
    }

    void Scene::InitManagers() {
        glCreateVertexArrays(1, &_rootVAO);
        glBindVertexArray(_rootVAO);

        _texManager = std::make_unique<TextureManager>();
        
        _matManager = std::make_unique<MaterialManager>([this](const std::string& path) {
            auto tex = _texManager->LoadTexture(path);
            return tex ? tex->GetHandle() : 0;
        });

        _modelManager = std::make_unique<ModelManager>([this](const auto& info) {
            return _matManager->RegisterMaterial(info);
        });

        _shaderManager = std::make_unique<ShaderManager>();
    }

    void Scene::InitShaders() {
        _shaderManager->LoadShader("ShaderVert", GL_VERTEX_SHADER, 
            Utils::GetAssetPath("AxonEngine/Shaders/shader.vert"));
    
        _shaderManager->LoadShader("ShaderFrag", GL_FRAGMENT_SHADER, 
            Utils::GetAssetPath("AxonEngine/Shaders/shader.frag"));

        _mainShader = _shaderManager->CreateProgram("MainShader", {"ShaderVert", "ShaderFrag"});
    }

    void Scene::InitFramebuffer() {
        _viewportFramebuffer = std::make_unique<Framebuffer>(1280, 720);
        
        TextureSpec colorSpec;
        colorSpec.internal_format = GL_RGBA8;
        colorSpec.format = GL_RGBA;
        _viewportFramebuffer->AddAttachment("Color", colorSpec, GL_COLOR_ATTACHMENT0);

        TextureSpec depthSpec;
        depthSpec.internal_format = GL_DEPTH_COMPONENT24;
        depthSpec.format = GL_DEPTH_COMPONENT;
        depthSpec.type = GL_FLOAT;
        _viewportFramebuffer->AddAttachment("Depth", depthSpec, GL_DEPTH_ATTACHMENT);

        _viewportFramebuffer->CheckStatus();
    }

    void Scene::InitRegistry() {
        auto myModel = _modelManager->LoadModel(Utils::GetAssetPath("External/Sponza/sponza.obj"));
        
        auto sponza = _registry.create();
        _registry.emplace<TransformComponent>(sponza, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.01f));
        _registry.emplace<ModelComponent>(sponza, myModel, true, true);
    }

    void Scene::OnUpdate(float deltaTime) {

    }

    void Scene::OnRender() {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        _matManager->SyncToGpu(2);

        _viewportFramebuffer->Bind();
        _viewportFramebuffer->Clear();

        float aspect = _viewportSize.x / (_viewportSize.y > 0 ? _viewportSize.y : 1.0f);
        glm::mat4 view = glm::lookAt(_camPos, _camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);

        _mainShader->Bind();
        _mainShader->SetUniform("view", view);
        _mainShader->SetUniform("proj", proj);

        for (auto [entity, transform, modelComp] : _registry.view<TransformComponent, ModelComponent>().each()) {
            if (!modelComp.isVisible) 
                continue;
            
            _mainShader->SetUniform("model", transform.GetTransform()); 
            _mainShader->SetUniform("modelIT", transform.GetTransformIT());

            if (modelComp.mesh) 
            {
                modelComp.mesh->Draw(_mainShader);
            }
        }

        _viewportFramebuffer->UnBind();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (_viewportSize.x != viewportPanelSize.x || _viewportSize.y != viewportPanelSize.y) {
            _viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
            _viewportFramebuffer->Resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
        }

        uint32_t textureID = _viewportFramebuffer->GetTexture("Color")->GetID();
        ImGui::Image((ImTextureID)textureID, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Begin("AxonEngine Settings");
        ImGui::Text("Sponza Explorer");
        ImGui::Separator();
        ImGui::DragFloat3("Camera Position", &_camPos.x, 0.1f);
        ImGui::DragFloat3("Camera Target", &_camTarget.x, 0.1f);
        ImGui::End();
    }

    Scene::~Scene() 
    {
        if (_rootVAO != 0) {
            glDeleteVertexArrays(1, &_rootVAO);
        }
    }
}