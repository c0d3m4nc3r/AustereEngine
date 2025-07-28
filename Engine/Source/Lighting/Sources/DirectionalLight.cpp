#include "Lighting/Sources.hpp"

namespace AE
{
    DirectionalLight::DirectionalLight(
        const Color& color,
        float intensity,
        const glm::vec3& direction,
        bool enabled
    ) : LightSource(color, intensity, enabled), direction(direction) {}

    void DirectionalLight::Apply(Shader* shader, const std::string& uniformName, int index)
    {
        if (!shader || uniformName.empty()) return;
    
        std::string prefix = uniformName + "[" + std::to_string(index) + "]";

        shader->SetVec3(prefix + ".color", color.ToVec3());
        shader->SetFloat(prefix + ".intensity", intensity);
        shader->SetVec3(prefix + ".direction", direction);
    }
    
    LightType DirectionalLight::GetType()
    {
        return LightType::Directional;
    }
}
