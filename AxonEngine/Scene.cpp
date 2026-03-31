#include "Scene.h"
#include <GL/glew.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Component/TransformComponent.h"
#include "Component/ModelComponent.h"
#include "Component/MaterialComponent.h"
#include "Manager/MaterialManager.h"
#include <iostream>
#include <random>
#include "Utils.h"

namespace Axn
{
    Scene::Scene(std::shared_ptr<InputManager> input) : 
        _inputManager(input)
    {
        InitManagers();
        InitShaders();
        InitRegistry();
        InitFramebuffer();
        _camera = std::make_unique<Camera>(input);
    }

    void Scene::InitManagers() {
        glCreateVertexArrays(1, &_rootVAO);
        glBindVertexArray(_rootVAO);

        _textureManager = std::make_unique<TextureManager>();
        
        _materialManager = std::make_unique<MaterialManager>([this](const std::string& path) {
            auto tex = _textureManager->LoadTexture(path);
            return tex ? tex->GetHandle() : 0;
        });

        _modelManager = std::make_unique<ModelManager>([this](const auto& info) {
            return _materialManager->RegisterMaterial(info);
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
        {
            auto sponzaModel = _modelManager->LoadModel(Utils::GetAssetPath("External/Sponza/sponza.obj"));
        
            auto sponza = _registry.create();
            _registry.emplace<TransformComponent>(sponza, TransformComponent{
                .position = glm::vec3(0.f, 0.f, 0.f),
                .rotation = glm::vec3(0.f, 0.f, 0.f),
                .scale = glm::vec3(0.1f)
            });

            _registry.emplace<ModelComponent>(sponza, ModelComponent{
                .mesh = sponzaModel,
                .isVisible = true,
                .castShadow = true
            });

            _registry.emplace<MaterialComponent>(sponza, MaterialComponent{
                .materialIndex = UINT32_MAX
            });
        }

        {
            std::random_device rd;
            std::mt19937 gen(rd());
            
            std::uniform_real_distribution<float> posDist(-50.0f, 50.0f);
            std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);
            std::uniform_real_distribution<float> scaleDist(1.0f, 3.5f);
            std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);

            std::vector<uint32_t> randomMaterials;
            for (int i = 0; i < 10; ++i) {
                Axn::MaterialInfo info;
                info.name = "RandomMat_" + std::to_string(i);
                info.baseColor = glm::vec4(colorDist(gen), colorDist(gen), colorDist(gen), 1.0f);
                randomMaterials.push_back(_materialManager->RegisterMaterial(info));
            }

            std::uniform_int_distribution<int> matDist(0, randomMaterials.size() - 1);

            auto cubeModel   = _modelManager->LoadModel(ModelManager::Cube);
            auto sphereModel = _modelManager->LoadModel(ModelManager::Sphere);
            auto torusModel  = _modelManager->LoadModel(ModelManager::Torus);

            auto spawnRandomEntity = [&](auto modelHandle) {
                auto entity = _registry.create();
                
                _registry.emplace<TransformComponent>(entity, TransformComponent{
                    .position = glm::vec3(posDist(gen), glm::abs(posDist(gen)), posDist(gen)),
                    .rotation = glm::vec3(rotDist(gen), rotDist(gen), rotDist(gen)),
                    .scale = glm::vec3(scaleDist(gen))
                });

                _registry.emplace<ModelComponent>(entity, ModelComponent{
                    .mesh = modelHandle,
                    .isVisible = true,
                    .castShadow = true
                });

                _registry.emplace<MaterialComponent>(entity, MaterialComponent{
                    .materialIndex = randomMaterials[matDist(gen)]
                });
            };

            for(int i = 0; i < 50; ++i)
            {
                spawnRandomEntity(cubeModel);
                spawnRandomEntity(sphereModel);
                spawnRandomEntity(torusModel);
            }
        }
    }

    void Scene::OnUpdate(float deltaTime) {
        _camera->OnUpdate(deltaTime, _isViewportHovered);
    }

    void Scene::OnRender() {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        _materialManager->SyncToGpu(2);

        _viewportFramebuffer->Bind();
        _viewportFramebuffer->Clear();

        _mainShader->Bind();
        _mainShader->SetUniform("view", _camera->GetViewMatrix());
        _mainShader->SetUniform("proj", _camera->GetProjectionMatrix());

        for (auto [entity, transform, modelComp, materialComp] : _registry.view<TransformComponent, ModelComponent, MaterialComponent>().each()) {
            if (!modelComp.isVisible) 
                continue;
            
            _mainShader->SetUniform("model", transform.GetTransform()); 
            _mainShader->SetUniform("modelIT", transform.GetTransformIT()); 

            if(materialComp.materialIndex != UINT32_MAX)
                modelComp.mesh->SetMaterial(0, materialComp.materialIndex);

            modelComp.mesh->Draw(_mainShader);
        }

        _viewportFramebuffer->UnBind();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        _isViewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (_viewportSize.x != viewportPanelSize.x || _viewportSize.y != viewportPanelSize.y) {
            _viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
            _viewportFramebuffer->Resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
            _camera->OnResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
        }

        uint32_t textureID = _viewportFramebuffer->GetTexture("Color")->GetID();
        ImGui::Image((ImTextureID)textureID, viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Begin("AxonEngine Settings");
        ImGui::End();
    }

    Scene::~Scene() 
    {
        if (_rootVAO != 0) {
            glDeleteVertexArrays(1, &_rootVAO);
        }
    }
}