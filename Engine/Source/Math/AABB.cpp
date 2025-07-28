#include "Math/AABB.hpp"

namespace AE
{
    AABB::AABB() : min(0.0f), max(0.0f) {}
    AABB::AABB(const glm::vec3& min, const glm::vec3& max)
        : min(min), max(max) {}

    void AABB::Expand(const glm::vec3& point)
    {
        min = glm::min(min, point);
        max = glm::max(max, point);
    }
    
    void AABB::Expand(const AABB& other)
    {
        min = glm::min(min, other.min);
        max = glm::max(max, other.max);
    }

    glm::vec3 AABB::GetCenter() const
    {
        return (min + max) * 0.5f;
    }
    
    glm::vec3 AABB::GetExtents() const
    {
        return (max - min) * 0.5f;
    }

    std::array<glm::vec3, 8> AABB::GetVertices() const {
        return {
            glm::vec3(min.x, min.y, min.z),
            glm::vec3(min.x, min.y, max.z),
            glm::vec3(min.x, max.y, min.z),
            glm::vec3(min.x, max.y, max.z),
            glm::vec3(max.x, min.y, min.z),
            glm::vec3(max.x, min.y, max.z),
            glm::vec3(max.x, max.y, min.z),
            glm::vec3(max.x, max.y, max.z)
        };
    }

    bool AABB::Contains(const glm::vec3& point) const
    {
        return (point.x >= min.x) && (point.x <= max.x) &&
               (point.y >= min.y) && (point.y <= max.y) &&
               (point.z >= min.z) && (point.z <= max.z);
    }
    
    bool AABB::Intersects(const AABB& other) const
    {
        return (min.x <= other.max.x) && (max.x >= other.min.x) &&
               (min.y <= other.max.y) && (max.y >= other.min.y) &&
               (min.z <= other.max.z) && (max.z >= other.min.z);
    }

    AABB AABB::Transform(const glm::mat4& matrix) const
    {
        std::array<glm::vec3, 8> vertices = GetVertices();
        AABB result;
        
        for (const auto& vertex : vertices)
        {
            glm::vec4 transformed = matrix * glm::vec4(vertex, 1.0f);
            result.Expand(glm::vec3(transformed) / transformed.w);
        }
        
        return result;
    }
}
