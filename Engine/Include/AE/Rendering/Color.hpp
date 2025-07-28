#pragma once

#include "PCH.hpp"

namespace AE
{
    typedef struct Color
    {
        float r, g, b, a;
    
        Color(float r, float g, float b, float a = 1.0f);
    
        glm::vec3 ToVec3() const;
        glm::vec4 ToVec4() const;

        static const Color White;
        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Transparent;
        static const Color Yellow;
        static const Color Cyan;
        static const Color Magenta;
    } Color;
}
