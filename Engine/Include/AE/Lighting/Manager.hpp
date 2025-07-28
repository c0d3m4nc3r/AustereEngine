#pragma once

#include "PCH.hpp"

#include "Lighting/Sources.hpp"
#include "Resources/Shader.hpp"

namespace AE
{
    using LightID = uint32_t;

    class LightManager
    {
    public:

        // template <typename T, typename... Args>
        // LightID CreateLight(Args&&... args);

        LightID AddLight(std::unique_ptr<LightSource> light);
        void RemoveLight(LightID id);
        void Clear();
        
        void Apply(Shader* shader);
        
        template <typename T>
        T* GetLight(LightID id);

        template <typename T>
        const T* GetLight(LightID id) const;
        
        size_t GetLightCount() const;

    private:

        std::unordered_map<LightID, std::unique_ptr<LightSource>> _lights;
        LightID _nextID = 0;
    };
}

#include "Lighting/Manager.inl"
