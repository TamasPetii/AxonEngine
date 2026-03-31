#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Axn 
{
    Camera::Camera(std::shared_ptr<InputManager> inputManager, float fov, float nearP, float farP)
        : _inputManager(inputManager), _fov(fov), _nearPlane(nearP), _farPlane(farP) 
    {
        UpdateCameraVectors();
    }

    void Camera::OnUpdate(float dt, bool isViewportHovered)
    {
        if (!_inputManager) return;

        if (isViewportHovered && _inputManager->IsButtonHeld(BUTTON_RIGHT)) 
        {
            auto [dx, dy] = _inputManager->GetMouseDelta();
            
            _yaw   += static_cast<float>(dx) * _mouseSensitivity;
            _pitch -= static_cast<float>(dy) * _mouseSensitivity;

            if (_pitch > 89.0f)  _pitch = 89.0f;
            if (_pitch < -89.0f) _pitch = -89.0f;

            UpdateCameraVectors();
        }

        float velocity = _movementSpeed * dt;
        
        if (_inputManager->IsKeyHeld(KEY_LEFT_SHIFT)) velocity *= 3.0f;
        if (_inputManager->IsKeyHeld(KEY_W)) _position += _front * velocity;
        if (_inputManager->IsKeyHeld(KEY_S)) _position -= _front * velocity;
        if (_inputManager->IsKeyHeld(KEY_A)) _position -= _right * velocity;
        if (_inputManager->IsKeyHeld(KEY_D)) _position += _right * velocity;
        if (_inputManager->IsKeyHeld(KEY_E)) _position += _up * velocity;
        if (_inputManager->IsKeyHeld(KEY_Q)) _position -= _up * velocity;
    }

    void Camera::OnResize(uint32_t width, uint32_t height) 
    {
        if (height > 0)
            _aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    }

    glm::mat4 Camera::GetViewMatrix() const 
    {
        return glm::lookAt(_position, _position + _front, _up);
    }

    glm::mat4 Camera::GetProjectionMatrix() const 
    {
        return glm::perspective(glm::radians(_fov), _aspectRatio, _nearPlane, _farPlane);
    }

    void Camera::UpdateCameraVectors() 
    {
        glm::vec3 front;
        front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        front.y = sin(glm::radians(_pitch));
        front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        
        _front = glm::normalize(front);
        _right = glm::normalize(glm::cross(_front, _worldUp));
        _up    = glm::normalize(glm::cross(_right, _front));
    }
}