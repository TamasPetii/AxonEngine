#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "../Manager/InputManager.h"

namespace Axn 
{
    class Camera 
    {
    public:
        Camera(std::shared_ptr<InputManager> inputManager, float fov = 45.0f, float nearP = 0.1f, float farP = 1000.0f);

        void OnUpdate(float dt, bool isViewportHovered);
        void OnResize(uint32_t width, uint32_t height);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
        const glm::vec3& GetPosition() const { return _position; }
        const glm::vec3& GetFront() const { return _front; }
    private:
        void UpdateCameraVectors();
    private:
        std::shared_ptr<InputManager> _inputManager;

        glm::vec3 _position{ 0.0f, 2.0f, 5.0f };
        glm::vec3 _front{ 0.0f, 0.0f, -1.0f };
        glm::vec3 _up{ 0.0f, 1.0f, 0.0f };
        glm::vec3 _right;
        glm::vec3 _worldUp{ 0.0f, 1.0f, 0.0f };

        float _yaw = -90.0f;
        float _pitch = 0.0f;

        float _fov;
        float _aspectRatio = 1.77f;
        float _nearPlane;
        float _farPlane;

        float _movementSpeed = 25.0f;
        float _mouseSensitivity = 0.2f;

        bool _isActive = false;
    };
}