#include "Lighting/Manager.hpp"

namespace AE
{
    template <typename T>
    T* LightManager::GetLight(LightID id)
    {
        static_assert(std::is_base_of_v<LightSource, T>, "T must inherit from LightSource");
        
        auto it = _lights.find(id);
        if (it == _lights.end()) return nullptr;
        
        return dynamic_cast<T*>(it->second.get());
    }

    template <typename T>
    const T* LightManager::GetLight(LightID id) const
    {
        static_assert(std::is_base_of_v<LightSource, T>, "T must inherit from LightSource");
        
        auto it = _lights.find(id);
        if (it == _lights.end()) return nullptr;
        
        return dynamic_cast<const T*>(it->second.get());
    }
}