#pragma once

#include "Rendering/Color.hpp"

namespace AE
{
    class Shader;
    class Texture;
    class Material
    {
    public:

        Material(
            std::shared_ptr<Shader> shader = nullptr,
            const Color& ambientColor = Color(0.2f, 0.2f, 0.2f, 1.0f),
            const Color& diffuseColor = Color(0.8f, 0.8f, 0.8f, 1.0f),
            const Color& specularColor = Color(1.0f, 1.0f, 1.0f, 1.0f),
            float shininess = 32.0f
        );

        ~Material();

        void Apply(const std::string& uniformName = "u_Material") const;

        static Material* GetDefault();
        
        const Color& GetAmbientColor() const;
        const Color& GetDiffuseColor() const;
        const Color& GetSpecularColor() const;
        float GetShininess() const;
        
        std::shared_ptr<Texture> GetDiffuseTexture() const;
        std::shared_ptr<Texture> GetSpecularTexture() const;
        std::shared_ptr<Texture> GetEmissiveTexture() const;
        std::shared_ptr<Texture> GetNormalTexture() const;
        std::shared_ptr<Texture> GetOpacityTexture() const;

        std::shared_ptr<Shader> GetShader() const;

        bool HasDiffuseTexture() const;
        bool HasSpecularTexture() const;
        bool HasEmissiveTexture() const;
        bool HasNormalTexture() const;
        bool HasOpacityTexture() const;

        bool IsTransparent() const;

        void SetAmbientColor(const Color& ambientColor);
        void SetDiffuseColor(const Color& diffuseColor);
        void SetSpecularColor(const Color& specularColor);
        void SetShininess(float shininess);

        void SetDiffuseTexture(std::shared_ptr<Texture> texture);
        void SetSpecularTexture(std::shared_ptr<Texture> texture);
        void SetEmissiveTexture(std::shared_ptr<Texture> texture);
        void SetNormalTexture(std::shared_ptr<Texture> texture);
        void SetOpacityTexture(std::shared_ptr<Texture> texture);

        void SetShader(std::shared_ptr<Shader> shader);

    private:

        Color _ambientColor;
        Color _diffuseColor;
        Color _specularColor;
        float _shininess;

        std::shared_ptr<Texture> _diffuseTexture;
        std::shared_ptr<Texture> _specularTexture;
        std::shared_ptr<Texture> _emissiveTexture;
        std::shared_ptr<Texture> _normalTexture;
        std::shared_ptr<Texture> _opacityTexture;

        std::shared_ptr<Shader> _shader;
    };
}
