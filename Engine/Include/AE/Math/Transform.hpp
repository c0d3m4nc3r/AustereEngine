#pragma once

#include "PCH.hpp"

#include <functional>

namespace AE
{
    class Transform
    {
    public:
        using DirtyCallback = std::function<void()>;
    
        Transform(
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::quat& rotation = glm::quat(glm::vec3(0.0f)),
            const glm::vec3& scale = glm::vec3(1.0f),
            Transform* parent = nullptr
        );
    
        ~Transform();
        
        void Translate(const glm::vec3& translation);
        void Rotate(const glm::quat& rotation);
        void Rotate(const glm::vec3& axis, float angle);
        void Scale(const glm::vec3& scaling);

        Transform* GetParent() const;
        
        const glm::vec3& GetPosition() const;
        const glm::quat& GetRotation() const;
        const glm::vec3& GetScale() const;
        
        glm::vec3 GetWorldPosition();
        glm::quat GetWorldRotation();
        glm::vec3 GetWorldScale();
    
        const glm::vec3& GetForward();
        const glm::vec3& GetRight();
        const glm::vec3& GetUp();

        const glm::mat4& GetLocalMatrix();
        glm::mat4 GetWorldMatrix();
    
        bool IsDirty() const;

        void SetParent(Transform* parent);

        void SetDirtyCallback(DirtyCallback callback);
    
        void SetPosition(const glm::vec3& position);
        void SetRotation(const glm::quat& rotation);
        void SetScale(const glm::vec3& scale);
    
        void SetDirty(bool dirty = true);

    private:

        Transform* _parent;
        std::vector<Transform*> _children;
    
        glm::vec3 _position, _scale;
        glm::quat _rotation;
        glm::vec3 _forward, _right, _up;

        glm::mat4 _localMatrix;
        
        DirtyCallback _callback;
    
        bool _dirty = true;
    
        void _UpdateVectors();
        void _UpdateLocalMatrix();
        void _Update();
    
    };
}
