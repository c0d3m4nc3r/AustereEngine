#include "Lighting/Manager.hpp"

namespace AE
{
    template <typename T>
    T* LightManager::GetLight(const std::string& name)
    {
        static_assert(std::is_base_of_v<LightSource, T>, "T must inherit from LightSource");
        
        auto it = _lights.find(name);
        if (it == _lights.end()) return nullptr;
        
        return dynamic_cast<T*>(it->second.get());
    }
}