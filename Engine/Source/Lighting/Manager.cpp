#include "Lighting/Manager.hpp"

namespace AE
{
    LightID LightManager::AddLight(std::unique_ptr<LightSource> light)
    {
        LightID id = _nextID++;
        _lights[id] = std::move(light);
        return id;
    }

    void LightManager::RemoveLight(LightID id)
    {
        _lights.erase(id);
    }

    void LightManager::Clear()
    {
        _lights.clear();
        _nextID = 0;
    }

    void LightManager::Apply(Shader* shader)
    {
        std::vector<LightSource*> directionalLights;
        std::vector<LightSource*> pointLights;
        std::vector<LightSource*> spotLights;

        for (auto& [id, light] : _lights)
        {
            if (!light->enabled) continue;
            
            switch (light->GetType())
            {
                case LightType::Directional:
                    directionalLights.push_back(light.get());
                    break;
                case LightType::Point:
                    pointLights.push_back(light.get());
                    break;
                case LightType::Spot:
                    spotLights.push_back(light.get());
                    break;
            }
        }

        shader->SetInt("u_DirLightCount", static_cast<int>(directionalLights.size()));
        shader->SetInt("u_PointLightCount", static_cast<int>(pointLights.size()));
        shader->SetInt("u_SpotLightCount", static_cast<int>(spotLights.size()));

        for (size_t i = 0; i < directionalLights.size(); ++i)
        {
            directionalLights[i]->Apply(shader, "u_DirLights", static_cast<int>(i));
        }

        for (size_t i = 0; i < pointLights.size(); ++i)
        {
            pointLights[i]->Apply(shader, "u_PointLights", static_cast<int>(i));
        }

        for (size_t i = 0; i < spotLights.size(); ++i)
        {
            spotLights[i]->Apply(shader, "u_SpotLights", static_cast<int>(i));
        }
    }

    size_t LightManager::GetLightCount() const
    {
        return _lights.size();
    }
}
