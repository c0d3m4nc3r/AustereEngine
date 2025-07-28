#pragma once

#include "Rendering/Color.hpp"
#include "Resources/Shader.hpp"

namespace AE
{
    enum class LightType { Directional, Point, Spot };

    class LightSource
    {
    public:

        LightSource(
            const Color& color = Color::White,
            float intensity = 1.0f,
            bool enabled = true
        ) : color(color), intensity(intensity), enabled(enabled) {}

        virtual ~LightSource() = default;

        Color color;
        float intensity;
        bool enabled;

        virtual void Apply(Shader* shader, const std::string& uniformName, int index = 0) = 0;
        
        virtual LightType GetType() = 0;

    };

    class DirectionalLight : public LightSource
    {
    public:

        DirectionalLight(
            const Color& color = Color::White,
            float intensity = 1.0f,
            const glm::vec3& direction = glm::vec3(0.0f, 0.0f, 1.0f),
            bool enabled = true
        );

        glm::vec3 direction;

        void Apply(Shader* shader, const std::string& uniformName, int index) override;
        
        LightType GetType() override;
    };

    class PointLight : public LightSource
    {
    public:

        PointLight(
            const Color& color = Color::White,
            float intensity = 1.0f,
            const glm::vec3& position = {0.0f, 0.0f, 0.0f},
            float constant = 1.0f,
            float linear = 0.09f,
            float quadratic = 0.032f,
            bool enabled = true
        );

        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;

        void Apply(Shader* shader, const std::string& uniformName, int index) override;

        LightType GetType() override;
    };

    class SpotLight : public LightSource
    {
    public:

        SpotLight(
            const Color& color = Color::White,
            float intensity = 1.0f,
            const glm::vec3& position = {0.0f, 0.0f, 0.0f},
            const glm::vec3& direction = { 1.0f, 0.0f, 0.0f},
            float innerCutoff = glm::cos(glm::radians(12.5f)),
            float outerCutoff = glm::cos(glm::radians(15.0f)),
            float constant = 1.0f,
            float linear = 0.09f,
            float quadratic = 0.032f,
            bool enabled = true
        );

        glm::vec3 position, direction;
        float innerCutoff, outerCutoff;
        float constant, linear, quadratic;

        void Apply(Shader* shader, const std::string& uniformName, int index) override;

        LightType GetType() override;
    };
}
