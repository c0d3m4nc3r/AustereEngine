#pragma once

#include "PCH.hpp"

namespace AE
{
    class AABB
    {
    public:

        AABB();
        AABB(const glm::vec3& min, const glm::vec3& max);

        glm::vec3 min, max;

        void Expand(const glm::vec3& point);
        void Expand(const AABB& other);

        glm::vec3 GetCenter() const;
        glm::vec3 GetExtents() const;

        std::array<glm::vec3, 8> GetVertices() const;

        bool Contains(const glm::vec3& point) const;
        bool Intersects(const AABB& other) const;

        AABB Transform(const glm::mat4& matrix) const;

    };
}
