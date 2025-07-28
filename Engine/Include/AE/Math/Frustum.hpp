#pragma once

#include "PCH.hpp"

namespace AE
{
    class AABB;
    class Frustum
    {
    public:

        void Update(const glm::mat4& viewProjection);

        bool Contains(const glm::vec3& point) const;
        bool Intersects(const glm::vec3& center, float radius) const;
        bool Intersects(const AABB& aabb) const;
    
    private:
        enum FrustumPlane {
            Left = 0,
            Right,
            Bottom,
            Top,
            Near,
            Far,
            Count
        };
        
        struct FrustumPlaneData {
            glm::vec3 normal;
            float distance;
        };
        
        std::array<FrustumPlaneData, FrustumPlane::Count> _planes;
    };
}
