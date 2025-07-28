#include "Scene/Manager.hpp"
#include "Scene/Scene.hpp"
#include "Core/Logger.hpp"

namespace AE
{
    SceneManager::SceneManager(Engine* engine) : _engine(engine) {}

    bool SceneManager::AddScene(std::shared_ptr<Scene> scene)
    {
        LoggerContext ctx("SceneManager", "AddScene");

        if (!scene)
        {
            Logger::Error("Attempt to add null scene!");
            return false;
        }

        if (_scenes.find(scene->GetName()) != _scenes.end())
        {
            Logger::Error("Scene with name '{}' already exists!", scene->GetName());
            return false;
        }

        scene->_SetEngine(_engine);
        _scenes[scene->GetName()] = scene;

        return true;
    }

    bool SceneManager::RemoveScene(const std::string& name)
    {
        LoggerContext ctx("SceneManager", "RemoveScene");

        auto it = _scenes.find(name);
        if (it == _scenes.end())
        {
            Logger::Warning("Scene '{}' not found!", name);
            return false;
        }
        
        if (_activeScene.lock() == it->second)
        {
            Logger::Debug("Removing active scene '{}', resetting active scene!", name);
            _activeScene.reset();
        }
        
        _scenes.erase(it);

        return true;
    }

    bool SceneManager::RemoveScene(const std::shared_ptr<Scene>& scene)
    {
        LoggerContext ctx("SceneManager", "RemoveScene");
        
        if (!scene)
        {
            Logger::Error("Attempt to remove null scene!");
            return false;
        }

        return RemoveScene(scene->GetName());
    }

    std::shared_ptr<Scene> SceneManager::GetScene(const std::string& name) const
    {
        LoggerContext ctx("SceneManager", "GetScene");

        auto it = _scenes.find(name);
        if (it == _scenes.end())
        {
            Logger::Warning("Scene '{}' not found!", name);
            return nullptr;
        }
            
        return it->second;
    }
    
    int SceneManager::GetScenesCount() const { return _scenes.size(); }
    
    std::shared_ptr<Scene> SceneManager::GetActiveScene() const { return _activeScene.lock(); }

    void SceneManager::SetActiveScene(const std::string& name)
    {
        LoggerContext ctx("SceneManager", "SetActiveScene");
        Logger::Info("Setting active scene to '{}'", name);

        auto scene = GetScene(name);
        if (scene)
        {
            if (!scene->IsInitialized())
                scene->_Initialize();

            if (auto currentActive = _activeScene.lock())
            {
                Logger::Debug("Deactivating current active scene '{}'!", currentActive->GetName());
                currentActive->_state.active = false;
            }
            
            _activeScene = scene;
            scene->_state.active = true;
            Logger::Info("Scene '{}' is now active!", name);
        }
        else
        {
            Logger::Error("Failed to set active scene - '{}' not found!", name);
        }
    }
    
    bool SceneManager::HasScene(const std::string& name) 
    { 
        return _scenes.find(name) != _scenes.end(); 
    }
    
    bool SceneManager::HasScene(std::shared_ptr<Scene> scene) 
    {
        if (!scene) return false;
        return _scenes.find(scene->GetName()) != _scenes.end();
    }

    void SceneManager::_Update()
    {
        const auto scene = _activeScene.lock();
        if (scene) scene->_Update();
    }

    void SceneManager::_Render()
    {
        const auto scene = _activeScene.lock();
        if (scene) scene->_Render();
    }
}