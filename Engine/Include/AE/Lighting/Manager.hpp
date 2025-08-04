#pragma once

#include "PCH.hpp"

#include "Lighting/Sources.hpp"
#include "Resources/Shader.hpp"

namespace AE
{
    class LightManager
    {
    public:
        
        bool AddLight(const std::string& name, std::unique_ptr<LightSource> light);
        bool RemoveLight(const std::string& name);
        
        void Apply(Shader* shader);
        
        template <typename T>
        T* GetLight(const std::string& name);
        
        size_t GetLightCount() const;

        bool HasLight(const std::string& name) const;

    private:

        std::unordered_map<std::string, std::unique_ptr<LightSource>> _lights;
    };
}

#include "Lighting/Manager.inl"
