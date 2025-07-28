#include "Math/Frustum.hpp"
#include "Math/AABB.hpp"

namespace AE
{
    void Frustum::Update(const glm::mat4& viewProjection)
    {
        // Left plane
        _planes[Left].normal.x = viewProjection[0][3] + viewProjection[0][0];
        _planes[Left].normal.y = viewProjection[1][3] + viewProjection[1][0];
        _planes[Left].normal.z = viewProjection[2][3] + viewProjection[2][0];
        _planes[Left].distance = viewProjection[3][3] + viewProjection[3][0];
        
        // Right plane
        _planes[Right].normal.x = viewProjection[0][3] - viewProjection[0][0];
        _planes[Right].normal.y = viewProjection[1][3] - viewProjection[1][0];
        _planes[Right].normal.z = viewProjection[2][3] - viewProjection[2][0];
        _planes[Right].distance = viewProjection[3][3] - viewProjection[3][0];
        
        // Bottom plane
        _planes[Bottom].normal.x = viewProjection[0][3] + viewProjection[0][1];
        _planes[Bottom].normal.y = viewProjection[1][3] + viewProjection[1][1];
        _planes[Bottom].normal.z = viewProjection[2][3] + viewProjection[2][1];
        _planes[Bottom].distance = viewProjection[3][3] + viewProjection[3][1];
        
        // Top plane
        _planes[Top].normal.x = viewProjection[0][3] - viewProjection[0][1];
        _planes[Top].normal.y = viewProjection[1][3] - viewProjection[1][1];
        _planes[Top].normal.z = viewProjection[2][3] - viewProjection[2][1];
        _planes[Top].distance = viewProjection[3][3] - viewProjection[3][1];
        
        // Near plane
        _planes[Near].normal.x = viewProjection[0][3] + viewProjection[0][2];
        _planes[Near].normal.y = viewProjection[1][3] + viewProjection[1][2];
        _planes[Near].normal.z = viewProjection[2][3] + viewProjection[2][2];
        _planes[Near].distance = viewProjection[3][3] + viewProjection[3][2];
        
        // Far plane
        _planes[Far].normal.x = viewProjection[0][3] - viewProjection[0][2];
        _planes[Far].normal.y = viewProjection[1][3] - viewProjection[1][2];
        _planes[Far].normal.z = viewProjection[2][3] - viewProjection[2][2];
        _planes[Far].distance = viewProjection[3][3] - viewProjection[3][2];
        
        for (auto& plane : _planes)
        {
            float length = glm::length(plane.normal);
            plane.normal /= length;
            plane.distance /= length;
        }
    }
    
    bool Frustum::Contains(const glm::vec3& point) const
    {
        for (const auto& plane : _planes)
        {
            if (glm::dot(plane.normal, point) + plane.distance < 0.0f)
                return false;
        }
        return true;
    }
    
    bool Frustum::Intersects(const glm::vec3& center, float radius) const
    {
        for (const auto& plane : _planes)
        {
            if (glm::dot(plane.normal, center) + plane.distance < -radius)
                return false;
        }
        return true;
    }
    
    bool Frustum::Intersects(const AABB& aabb) const
    {
        for (const auto& plane : _planes)
        {
            glm::vec3 positiveVertex = aabb.min;
            
            if (plane.normal.x >= 0) positiveVertex.x = aabb.max.x;
            if (plane.normal.y >= 0) positiveVertex.y = aabb.max.y;
            if (plane.normal.z >= 0) positiveVertex.z = aabb.max.z;
            
            if (glm::dot(plane.normal, positiveVertex) + plane.distance < 0.0f)
                return false;
        }
        return true;
    }
}
