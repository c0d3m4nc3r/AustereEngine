#include "Rendering/Camera.hpp"
#include "Core/Logger.hpp"

namespace AE
{
    Camera::Camera(
        const Transform& transform,
        float aspectRatio,
        float fieldOfView,
        float nearPlane,
        float farPlane
    ) : transform(transform),
        _fieldOfView(fieldOfView),
        _nearPlane(nearPlane),
        _farPlane(farPlane),
        _aspectRatio(aspectRatio)
    {
        this->transform.SetDirtyCallback([this]() { _dirty = true; });
    }
    
    Camera::~Camera() {}

    const Frustum& Camera::GetFrustum() const { return _frustum; }
    
    float Camera::GetFieldOfView() const { return _fieldOfView; }
    float Camera::GetNearPlane() const { return _nearPlane; }
    float Camera::GetFarPlane() const { return _farPlane; }
    float Camera::GetAspectRatio() const { return _aspectRatio; }
    
    const glm::mat4& Camera::GetViewMatrix()
    {
        if (IsDirty()) _Update();
        return _viewMatrix;
    }
    
    const glm::mat4& Camera::GetProjectionMatrix()
    {
        if (IsDirty()) _Update();
        return _projectionMatrix;
    }
    
    bool Camera::IsDirty() const { return _dirty; }
    
    void Camera::SetFieldOfView(float fieldOfView)
    {
        if (_fieldOfView == fieldOfView) return;
        _fieldOfView = fieldOfView;
        SetDirty();
    }
    
    void Camera::SetNearPlane(float nearPlane)
    {
        if (_nearPlane == nearPlane) return;
        _nearPlane = nearPlane;
        SetDirty();
    }
    
    void Camera::SetFarPlane(float farPlane)
    {
        if (_farPlane == farPlane) return;
        _farPlane = farPlane;
        SetDirty();
    }
    
    void Camera::SetAspectRatio(float aspectRatio)
    {
        if (_aspectRatio == aspectRatio) return;
        _aspectRatio = aspectRatio;
        SetDirty();
    }
    
    void Camera::SetDirty(bool dirty)
    {
        _dirty = dirty;
    }
    
    void Camera::_UpdateViewMatrix()
    {
        glm::vec3 position = transform.GetWorldPosition();
        _viewMatrix = glm::lookAt(
            position,
            position + transform.GetForward(),
            transform.GetUp()
        );
    }
    
    void Camera::_UpdateProjectionMatrix()
    {
        _projectionMatrix = glm::perspective(
            glm::radians(_fieldOfView),
            _aspectRatio,
            _nearPlane,
            _farPlane
        );
    }
    
    void Camera::_Update()
    {
        _UpdateViewMatrix();
        _UpdateProjectionMatrix();
        _frustum.Update(_projectionMatrix * _viewMatrix);
        _dirty = false;
    }
}
