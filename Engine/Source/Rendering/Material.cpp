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
        SPtr<Shader> shader,
        const Color& ambientColor,
        const Color& diffuseColor,
        const Color& specularColor,
        float shininess
    ) : _shader(shader),
        _ambientColor(ambientColor),
        _diffuseColor(diffuseColor),
        _specularColor(specularColor),
        _shininess(shininess)
    {}

    Material::~Material() {}

    void Material::Apply(const std::string& uniformName) const
    {
        if (_shader == nullptr) return;
        
        _shader->Bind();

        _shader->SetVec3(uniformName + ".ambientColor", _ambientColor.ToVec3());
        _shader->SetVec3(uniformName + ".diffuseColor", _diffuseColor.ToVec3());
        _shader->SetVec3(uniformName + ".specularColor", _specularColor.ToVec3());
        _shader->SetFloat(uniformName + ".shininess", _shininess);

        // Diffuse
        if (HasDiffuseTexture() && _diffuseTexture->IsValid()) {
            _diffuseTexture->Bind(0);
            _shader->SetInt(uniformName + ".diffuseTexture", DIFFUSE_TEXTURE_SLOT);
            _shader->SetBool(uniformName + ".hasDiffuseTexture", true);
        } else {
            _shader->SetBool(uniformName + ".hasDiffuseTexture", false);
        }

        // Specular
        if (HasSpecularTexture() && _specularTexture->IsValid()) {
            _specularTexture->Bind(1);
            _shader->SetInt(uniformName + ".specularTexture", SPECULAR_TEXTURE_SLOT);
            _shader->SetBool(uniformName + ".hasSpecularTexture", true);
        } else {
            _shader->SetBool(uniformName + ".hasSpecularTexture", false);
        }

        // Emissive
        if (HasEmissiveTexture() && _emissiveTexture->IsValid()) {
            _emissiveTexture->Bind(2);
            _shader->SetInt(uniformName + ".emissiveTexture", EMISSIVE_TEXTURE_SLOT);
            _shader->SetBool(uniformName + ".hasEmissiveTexture", true);
        } else {
            _shader->SetBool(uniformName + ".hasEmissiveTexture", false);
        }

        // Normal
        if (HasNormalTexture() && _normalTexture->IsValid()) {
            _normalTexture->Bind(3);
            _shader->SetInt(uniformName + ".normalTexture", NORMAL_TEXTURE_SLOT);
            _shader->SetBool(uniformName + ".hasNormalTexture", true);
        } else {
            _shader->SetBool(uniformName + ".hasNormalTexture", false);
        }

        // Opacity
        if (HasOpacityTexture() && _opacityTexture->IsValid()) {
            _opacityTexture->Bind(4);
            _shader->SetInt(uniformName + ".opacityTexture", OPACITY_TEXTURE_SLOT);
            _shader->SetBool(uniformName + ".hasOpacityTexture", true);
        } else {
            _shader->SetBool(uniformName + ".hasOpacityTexture", false);
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

    SPtr<Texture> Material::GetDiffuseTexture() const { return _diffuseTexture; }
    SPtr<Texture> Material::GetSpecularTexture() const { return _specularTexture; }
    SPtr<Texture> Material::GetEmissiveTexture() const { return _emissiveTexture; }
    SPtr<Texture> Material::GetNormalTexture() const { return _normalTexture; }
    SPtr<Texture> Material::GetOpacityTexture() const { return _opacityTexture; }

    SPtr<Shader> Material::GetShader() const { return _shader; }

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

    void Material::SetDiffuseTexture(SPtr<Texture> texture) { _diffuseTexture = texture; }
    void Material::SetSpecularTexture(SPtr<Texture> texture) { _specularTexture = texture; }
    void Material::SetEmissiveTexture(SPtr<Texture> texture) { _emissiveTexture = texture; }
    void Material::SetNormalTexture(SPtr<Texture> texture) { _normalTexture = texture; }
    void Material::SetOpacityTexture(SPtr<Texture> texture) { _opacityTexture = texture; }

    void Material::SetShader(SPtr<Shader> shader) { _shader = shader; }
}
