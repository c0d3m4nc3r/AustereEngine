#pragma once

#include "PCH.hpp"

#include "Math/Transform.hpp"
#include "Math/Frustum.hpp"

namespace AE
{
    class Camera
    {
    public:
    
        Camera(
            const Transform& transform = Transform(),
            float aspectRatio = 16.0f / 9.0f,
            float fieldOfView = 45.0f,
            float nearPlane = 0.1f,
            float farPlane = 1024.0f
        );
    
        ~Camera();
    
        Transform transform;

        const Frustum& GetFrustum() const;
    
        float GetFieldOfView() const;
        float GetNearPlane() const;
        float GetFarPlane() const;
        float GetAspectRatio() const;
    
        const glm::mat4& GetViewMatrix();
        const glm::mat4& GetProjectionMatrix();
    
        bool IsDirty() const;
    
        void SetFieldOfView(float fieldOfView);
        void SetNearPlane(float nearPlane);
        void SetFarPlane(float farPlane);
        void SetAspectRatio(float aspectRatio);
    
        void SetDirty(bool dirty = true);
    
    private:

        Frustum _frustum;
    
        float _fieldOfView;
        float _nearPlane, _farPlane;
        float _aspectRatio;
    
        glm::mat4 _viewMatrix, _projectionMatrix;
    
        bool _dirty = true;
    
        void _UpdateViewMatrix();
        void _UpdateProjectionMatrix();
        void _Update();
    };
}
