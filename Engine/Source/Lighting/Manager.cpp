#include "Lighting/Manager.hpp"
#include "Core/Logger.hpp"

namespace AE
{
    bool LightManager::AddLight(const std::string& name, std::unique_ptr<LightSource> light)
    {
        LoggerContext ctx("LightManager", "AddLight");

        auto [it, inserted] = _lights.emplace(name, std::move(light));
        if (!inserted)
        {
            Logger::Error("Light source with name '{}' already exists!", name);
            return false;
        }

        Logger::Debug("Added light source named '{}'!", name);

        return true;
    }

    bool LightManager::RemoveLight(const std::string& name)
    {
        LoggerContext ctx("LightManager", "RemoveLight");

        auto it = _lights.find(name);
        if (it != _lights.end())
        {
            _lights.erase(it);
            Logger::Debug("Removed light source named '{}'!", name);
            return true;
        }

        Logger::Error("Light source with name '{}' not found!", name);

        return false;
    }

    void LightManager::Apply(Shader* shader)
    {
        if (!shader) return;

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

    bool LightManager::HasLight(const std::string& name) const
    {
        return _lights.find(name) != _lights.end();
    }
}
