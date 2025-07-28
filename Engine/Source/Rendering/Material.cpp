#include "Rendering/Material.hpp"
#include "Resources/Shader.hpp"
#include "Resources/Texture.hpp"

namespace AE
{
    enum
    {
        DIFFUSE_TEXTURE_SLOT = 0,
        SPECULAR_TEXTURE_SLOT,
        EMISSIVE_TEXTURE_SLOT,
        NORMAL_TEXTURE_SLOT,
        OPACITY_TEXTURE_SLOT
    };

    Material::Material(
        const Color& ambientColor,
        const Color& diffuseColor,
        const Color& specularColor,
        float shininess
    ) : _ambientColor(ambientColor),
        _diffuseColor(diffuseColor),
        _specularColor(specularColor),
        _shininess(shininess)
    {}

    Material::~Material() {}

    void Material::Apply(Shader* shader, const std::string& uniformName) const
    {
        if (shader == nullptr) return;

        shader->Bind();

        shader->SetVec3(uniformName + ".ambientColor", _ambientColor.ToVec3());
        shader->SetVec3(uniformName + ".diffuseColor", _diffuseColor.ToVec3());
        shader->SetVec3(uniformName + ".specularColor", _specularColor.ToVec3());
        shader->SetFloat(uniformName + ".shininess", _shininess);

        // Diffuse
        if (HasDiffuseTexture() && _diffuseTexture->IsValid()) {
            _diffuseTexture->Bind(0);
            shader->SetInt(uniformName + ".diffuseTexture", DIFFUSE_TEXTURE_SLOT);
            shader->SetBool(uniformName + ".hasDiffuseTexture", true);
        } else {
            shader->SetBool(uniformName + ".hasDiffuseTexture", false);
        }

        // Specular
        if (HasSpecularTexture() && _specularTexture->IsValid()) {
            _specularTexture->Bind(1);
            shader->SetInt(uniformName + ".specularTexture", SPECULAR_TEXTURE_SLOT);
            shader->SetBool(uniformName + ".hasSpecularTexture", true);
        } else {
            shader->SetBool(uniformName + ".hasSpecularTexture", false);
        }

        // Emissive
        if (HasEmissiveTexture() && _emissiveTexture->IsValid()) {
            _emissiveTexture->Bind(2);
            shader->SetInt(uniformName + ".emissiveTexture", EMISSIVE_TEXTURE_SLOT);
            shader->SetBool(uniformName + ".hasEmissiveTexture", true);
        } else {
            shader->SetBool(uniformName + ".hasEmissiveTexture", false);
        }

        // Normal
        if (HasNormalTexture() && _normalTexture->IsValid()) {
            _normalTexture->Bind(3);
            shader->SetInt(uniformName + ".normalTexture", NORMAL_TEXTURE_SLOT);
            shader->SetBool(uniformName + ".hasNormalTexture", true);
        } else {
            shader->SetBool(uniformName + ".hasNormalTexture", false);
        }

        // Opacity
        if (HasOpacityTexture() && _opacityTexture->IsValid()) {
            _opacityTexture->Bind(4);
            shader->SetInt(uniformName + ".opacityTexture", OPACITY_TEXTURE_SLOT);
            shader->SetBool(uniformName + ".hasOpacityTexture", true);
        } else {
            shader->SetBool(uniformName + ".hasOpacityTexture", false);
        }
    }

    Material* Material::GetDefault()
    {
        static Material defaultMaterial;
        return &defaultMaterial;
    }

    const Color& Material::GetAmbientColor() const { return _ambientColor; }
    const Color& Material::GetDiffuseColor() const { return _diffuseColor; }
    const Color& Material::GetSpecularColor() const { return _specularColor; }
    float Material::GetShininess() const { return _shininess; }

    std::shared_ptr<Texture> Material::GetDiffuseTexture() const { return _diffuseTexture; }
    std::shared_ptr<Texture> Material::GetSpecularTexture() const { return _specularTexture; }
    std::shared_ptr<Texture> Material::GetEmissiveTexture() const { return _emissiveTexture; }
    std::shared_ptr<Texture> Material::GetNormalTexture() const { return _normalTexture; }
    std::shared_ptr<Texture> Material::GetOpacityTexture() const { return _opacityTexture; }

    bool Material::HasDiffuseTexture() const { return _diffuseTexture != nullptr; }
    bool Material::HasSpecularTexture() const { return _specularTexture != nullptr; }
    bool Material::HasEmissiveTexture() const { return _emissiveTexture != nullptr; }
    bool Material::HasNormalTexture() const { return _normalTexture != nullptr; }
    bool Material::HasOpacityTexture() const { return _opacityTexture != nullptr; }

    bool Material::IsTransparent() const
    {
        if (_opacityTexture && _opacityTexture->IsValid())
            return true;

        return _diffuseTexture && _diffuseTexture->HasTransparency();
    }

    void Material::SetAmbientColor(const Color& ambientColor) { _ambientColor = ambientColor; }
    void Material::SetDiffuseColor(const Color& diffuseColor) { _diffuseColor = diffuseColor; }
    void Material::SetSpecularColor(const Color& specularColor) { _specularColor = specularColor; }
    void Material::SetShininess(float shininess) { _shininess = shininess; }

    void Material::SetDiffuseTexture(std::shared_ptr<Texture> texture) { _diffuseTexture = texture; }
    void Material::SetSpecularTexture(std::shared_ptr<Texture> texture) { _specularTexture = texture; }
    void Material::SetEmissiveTexture(std::shared_ptr<Texture> texture) { _emissiveTexture = texture; }
    void Material::SetNormalTexture(std::shared_ptr<Texture> texture) { _normalTexture = texture; }
    void Material::SetOpacityTexture(std::shared_ptr<Texture> texture) { _opacityTexture = texture; }
}
