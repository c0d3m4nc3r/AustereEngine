#include "Math/Transform.hpp"

namespace AE
{
    Transform::Transform(
        const glm::vec3& position,
        const glm::quat& rotation,
        const glm::vec3& scale,
        Transform* parent
    ) : _position(position),
        _rotation(rotation),
        _scale(scale),
        _parent(parent) {}
    
    Transform::~Transform() {}
    
    void Transform::Translate(const glm::vec3& translation)
    {
        _position += translation;
        SetDirty();
    }
    
    void Transform::Rotate(const glm::quat& rotation)
    {
        _rotation *= rotation;
        SetDirty();
    }
    
    void Transform::Rotate(const glm::vec3& axis, float angle)
    {
        _rotation = glm::rotate(_rotation, angle, axis);
        SetDirty();
    }
    
    void Transform::Scale(const glm::vec3& scaling)
    {
        _scale *= scaling;
        SetDirty();
    }

    Transform* Transform::GetParent() const { return _parent; }
    
    const glm::vec3& Transform::GetPosition() const { return _position; }
    const glm::quat& Transform::GetRotation() const { return _rotation; }
    const glm::vec3& Transform::GetScale() const { return _scale; }

    glm::vec3 Transform::GetWorldPosition()
    {
        return glm::vec3(GetWorldMatrix()[3]);
    }

    glm::quat Transform::GetWorldRotation()
    {
        if (_parent)
        {
            return _parent->GetWorldRotation() * _rotation;
        }
        return _rotation;
    }

    glm::vec3 Transform::GetWorldScale()
    {
        const glm::mat4& mat = GetWorldMatrix();

        glm::vec3 scale;
        scale.x = glm::length(glm::vec3(mat[0]));
        scale.y = glm::length(glm::vec3(mat[1]));
        scale.z = glm::length(glm::vec3(mat[2]));
        
        return scale;
    }
    
    const glm::vec3& Transform::GetForward()
    {
        if (IsDirty()) _Update();
        return _forward;
    }
    
    const glm::vec3& Transform::GetRight()
    {
        if (IsDirty()) _Update();
        return _right;
    }
    
    const glm::vec3& Transform::GetUp()
    {
        if (IsDirty()) _Update();
        return _up;
    }
    
    const glm::mat4& Transform::GetLocalMatrix()
    {
        if (IsDirty()) _Update();
        return _localMatrix;
    }

    glm::mat4 Transform::GetWorldMatrix()
    {
        if (_parent)
        {
            return _parent->GetWorldMatrix() * GetLocalMatrix();
        }

        return GetLocalMatrix();
    }
    
    bool Transform::IsDirty() const { return _dirty; }

    void Transform::SetParent(Transform* parent)
    {
        if (_parent)
        {
            auto& siblings = _parent->_children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        }

        _parent = parent;

        if (_parent)
            _parent->_children.push_back(this);

        SetDirty();
    }
    
    void Transform::SetDirtyCallback(DirtyCallback callback)
    {
        _callback = callback;
    }
    
    void Transform::SetPosition(const glm::vec3& position)
    {
        _position = position;
        SetDirty();
    }
    
    void Transform::SetRotation(const glm::quat& rotation)
    {
        _rotation = rotation;
        SetDirty();
    }
    
    void Transform::SetScale(const glm::vec3& scale)
    {
        _scale = scale;
        SetDirty();
    }
    
    void Transform::SetDirty(bool dirty)
    {
        _dirty = dirty;
        
        if (dirty)
        {
            for (Transform* child : _children)
                child->SetDirty();

            if (_callback) _callback();
        }
    }
    
    void Transform::_UpdateVectors()
    {
        glm::quat worldRot = GetWorldRotation();
        _forward = glm::normalize(worldRot * glm::vec3(0, 0, -1));
        _right   = glm::normalize(worldRot * glm::vec3(1, 0, 0));
        _up      = glm::normalize(worldRot * glm::vec3(0, 1, 0));
    }
    
    void Transform::_UpdateLocalMatrix()
    {
        _localMatrix = glm::translate(glm::mat4(1.0f), _position);
        _localMatrix *= glm::mat4_cast(_rotation);
        _localMatrix = glm::scale(_localMatrix, _scale);
    }
    
    void Transform::_Update()
    {
        _UpdateLocalMatrix();
        _UpdateVectors();
        _dirty = false;
    }
}
