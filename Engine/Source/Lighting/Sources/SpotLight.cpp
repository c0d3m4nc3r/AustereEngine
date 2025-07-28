#include "Lighting/Sources.hpp"

namespace AE
{
    SpotLight::SpotLight(
        const Color& color,
        float intensity,
        const glm::vec3& position,
        const glm::vec3& direction,
        float innerCutoff,
        float outerCutoff,
        float constant,
        float linear,
        float quadratic,
        bool enabled
    ) : LightSource(color, intensity, enabled),
        position(position), direction(direction),
        innerCutoff(innerCutoff), outerCutoff(outerCutoff),
        constant(constant), linear(linear), quadratic(quadratic) {}

    void SpotLight::Apply(Shader* shader, const std::string& uniformName, int index)
    {
        if (!shader || uniformName.empty()) return;
    
        std::string prefix = uniformName + "[" + std::to_string(index) + "]";

        shader->SetVec3(prefix + ".color", color.ToVec3());
        shader->SetFloat(prefix + ".intensity", intensity);
        shader->SetVec3(prefix + ".position", position);
        shader->SetVec3(prefix + ".direction", direction);
        shader->SetFloat(prefix + ".innerCutoff", innerCutoff);
        shader->SetFloat(prefix + ".outerCutoff", outerCutoff);
        shader->SetFloat(prefix + ".constant", constant);
        shader->SetFloat(prefix + ".linear", linear);
        shader->SetFloat(prefix + ".quadratic", quadratic);
    }
    
    LightType SpotLight::GetType()
    {
        return LightType::Spot;
    }

}
