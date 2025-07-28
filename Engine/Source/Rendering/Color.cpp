#include "Rendering/Color.hpp"

namespace AE
{
    Color::Color(float r, float g, float b, float a)
        : r(r), g(g), b(b), a(a) {}

    glm::vec3 Color::ToVec3() const { return glm::vec3(r, g, b); }
    glm::vec4 Color::ToVec4() const { return glm::vec4(r, g, b, a); }

    const Color Color::White       = Color(1.0f, 1.0f, 1.0f, 1.0f);
    const Color Color::Black       = Color(0.0f, 0.0f, 0.0f, 1.0f);
    const Color Color::Red         = Color(1.0f, 0.0f, 0.0f, 1.0f);
    const Color Color::Green       = Color(0.0f, 1.0f, 0.0f, 1.0f);
    const Color Color::Blue        = Color(0.0f, 0.0f, 1.0f, 1.0f);
    const Color Color::Transparent = Color(0.0f, 0.0f, 0.0f, 0.0f);
    const Color Color::Yellow      = Color(1.0f, 1.0f, 0.0f, 1.0f);
    const Color Color::Cyan        = Color(0.0f, 1.0f, 1.0f, 1.0f);
    const Color Color::Magenta     = Color(1.0f, 0.0f, 1.0f, 1.0f);
}
