#include "Lighting/Sources.hpp"

namespace AE
{
    PointLight::PointLight(
        const Color& color,
        float intensity,
        const glm::vec3& position,
        float constant,
        float linear,
        float quadratic,
        bool enabled
    ) : LightSource(color, intensity, enabled),
        position(position),
        constant(constant),
        linear(linear),
        quadratic(quadratic) {}
    
    void PointLight::Apply(Shader* shader, const std::string& uniformName, int index)
    {
        if (!shader || uniformName.empty()) return;
    
        std::string prefix = uniformName + "[" + std::to_string(index) + "]";

        shader->SetVec3(prefix + ".color", color.ToVec3());
        shader->SetFloat(prefix + ".intensity", intensity);
        shader->SetVec3(prefix + ".position", position);
        shader->SetFloat(prefix + ".constant", constant);
        shader->SetFloat(prefix + ".linear", linear);
        shader->SetFloat(prefix + ".quadratic", quadratic);
    }
    
    LightType PointLight::GetType()
    {
        return LightType::Point;
    }
    
}
