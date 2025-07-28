#include "Scene/Scene.hpp"
#include "Scene/Node.hpp"
#include "Core/Logger.hpp"

namespace AE
{
    Scene::Scene(const std::string& name, std::shared_ptr<SceneNode> root)
        : _name(name)
    {
        if (!root)
            _root = std::make_shared<SceneNode>("Root");
    }

    Scene::~Scene()
    {
        if (_state.initialized)
            _Destroy();
    }

    const std::string& Scene::GetName() const { return _name; }

    std::shared_ptr<SceneNode> Scene::GetRoot() const { return _root; }
    void Scene::SetRoot(std::shared_ptr<SceneNode> node)
    {
        if (node)
        {
            node->_SetEngine(_engine);
            if (IsInitialized() && !node->IsInitialized())
                node->_Initialize();
        }
        _root = node;
    }

    bool Scene::IsInitialized() const { return _state.initialized; }
    bool Scene::IsActive() const { return _state.active; }

    bool Scene::_Initialize()
    {
        LoggerContext ctx("Scene(" + _name + ")", "_Initialize");

        Logger::Info("Initializing scene...");

        if (_state.initialized)
        {
            Logger::Warning("Scene is already initialized!");
            return false;
        }

        if (!_root)
        {
            Logger::Error("Root node is null!");
            return false;
        }
        
        if (!_root->_Initialize())
        {
            Logger::Error("Failed to initialize root node!");
            return false;
        }

        _state.initialized = true;

        Logger::Info("Scene initialized!");
        
        return true;
    }

    void Scene::_Destroy()
    {
        LoggerContext ctx("Scene(" + _name + ")", "_Destroy");

        Logger::Info("Destroying scene...");

        if (!_state.initialized)
        {
            Logger::Warning("Scene is not initialized! Aborting...");
            return;
        }

        if (_root)
        {
            _root->_Destroy();
            _root.reset();
        }

        _state.initialized = false;

        Logger::Info("Scene destroyed!");
    }
    
    void Scene::_Update()
    {
        if (!_root) return;

        _root->_Update();
    }
    
    void Scene::_Render()
    {
        if (!_root) return;

        _root->_Render();
    }

    void Scene::_SetEngine(Engine* engine)
    {
        _engine = engine;
        if (_root)
            _root->_SetEngine(engine);
    }
}
