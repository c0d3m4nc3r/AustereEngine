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
            SPtr<Shader> shader = nullptr,
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
        
        SPtr<Texture> GetDiffuseTexture() const;
        SPtr<Texture> GetSpecularTexture() const;
        SPtr<Texture> GetEmissiveTexture() const;
        SPtr<Texture> GetNormalTexture() const;
        SPtr<Texture> GetOpacityTexture() const;

        SPtr<Shader> GetShader() const;

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

        void SetDiffuseTexture(SPtr<Texture> texture);
        void SetSpecularTexture(SPtr<Texture> texture);
        void SetEmissiveTexture(SPtr<Texture> texture);
        void SetNormalTexture(SPtr<Texture> texture);
        void SetOpacityTexture(SPtr<Texture> texture);

        void SetShader(SPtr<Shader> shader);

    private:

        Color _ambientColor;
        Color _diffuseColor;
        Color _specularColor;
        float _shininess;

        SPtr<Texture> _diffuseTexture;
        SPtr<Texture> _specularTexture;
        SPtr<Texture> _emissiveTexture;
        SPtr<Texture> _normalTexture;
        SPtr<Texture> _opacityTexture;

        SPtr<Shader> _shader;
    };
}
