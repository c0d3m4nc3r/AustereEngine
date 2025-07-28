#include "Scene/Node.hpp"
#include "Core/Logger.hpp"

namespace AE
{
    SceneNode::SceneNode(const std::string& name)
        : _name(name) {}

    SceneNode::~SceneNode()
    {
        if (IsInitialized())
            _Destroy();
    }
    
    bool SceneNode::AddChild(std::shared_ptr<SceneNode> node)
    {
        LoggerContext ctx("SceneNode(" + _name + ")", "AddChild");

        if (!node)
        {
            Logger::Error("Cannot add null child to node \"{}\"!", _name);
            return false;
        }

        if (node.get() == this)
        {
            Logger::Error("Cannot add node \"{}\" as a child of itself!", _name);
            return false;
        }

        const std::string& name = node->GetName();

        if (_children.find(name) != _children.end())
        {
            Logger::Error("Node \"{}\" already has child \"{}\"!", _name, name);
            return false;
        }

        node->SetParent(shared_from_this());
        node->_SetEngine(engine);
        
        if (IsInitialized() && !node->IsInitialized())
            node->_Initialize();
        
        _children[name] = node;

        return true;
    }

    bool SceneNode::RemoveChild(const std::string& name)
    {
        LoggerContext ctx("SceneNode(" + _name + ")", "RemoveChild");

        auto it = _children.find(name);
        if (it == _children.end())
        {
            Logger::Error("Node \"{}\" has no child named \"{}\"!", _name, name);
            return false;
        }

        it->second->SetParent(nullptr);
        _children.erase(it);

        return true;
    }

    bool SceneNode::RemoveChild(const std::shared_ptr<SceneNode>& node)
    {
        LoggerContext ctx("SceneNode(" + _name + ")", "RemoveChild");

        if (!node)
        {
            Logger::Error("Cannot remove null child from node \"{}\"!", _name);
            return false;
        }

        const std::string& name = node->GetName();
        auto it = _children.find(name);
        
        if (it == _children.end() || it->second != node)
        {
            Logger::Warning("Node doesn't have specified child node!");
            return false;
        }

        node->SetParent(nullptr);
        _children.erase(it);

        return true;
    }

    const std::string& SceneNode::GetName() const { return _name; }

    Transform& SceneNode::GetTransform() { return transform; }
    const Transform& SceneNode::GetTransform() const { return transform; }
    void SceneNode::SetTransform(const Transform& transform) { this->transform = transform; }

    std::shared_ptr<SceneNode> SceneNode::GetChild(const std::string& name) const
    {
        auto it = _children.find(name);
        return it != _children.end() ? it->second : nullptr;
    }

    std::size_t SceneNode::GetChildrenCount() const { return _children.size(); }

    std::shared_ptr<SceneNode> SceneNode::GetParent() const { return _parent.lock(); }
    void SceneNode::SetParent(std::shared_ptr<SceneNode> node)
    {
        _parent = node;
        transform.SetParent(node ? &node->GetTransform() : nullptr);
    }
    bool SceneNode::HasParent() const { return !_parent.expired(); }
    
    bool SceneNode::HasChildren() const { return !_children.empty(); }

    bool SceneNode::HasChild(const std::string& name) const
    {
        return _children.find(name) != _children.end();
    }

    bool SceneNode::HasChild(const std::shared_ptr<SceneNode>& node) const
    {
        if (!node) return false;
        auto it = _children.find(node->GetName());
        return it != _children.end() && it->second == node;
    }
    
    bool SceneNode::IsInitialized() const { return _state.initialized; }
    bool SceneNode::IsEnabled() const { return _state.enabled; }
    void SceneNode::SetEnabled(bool enabled) { _state.enabled = enabled; }

    bool SceneNode::_Initialize() 
    {
        LoggerContext ctx("SceneNode(" + _name + ")", "_Initialize");

        if (_state.initialized) 
        {
            Logger::Warning("Node is already initialized!");
            return false;
        }

        bool success = OnInitialize();
        if (!success) 
        {
            Logger::Error("Node initialization failed!");
            return false;
        }

        for (auto& [name, child] : _children) 
        {
            child->engine = engine;
            if (!child->_Initialize()) 
            {
                success = false;
                break;
            }
        }

        if (!success) 
        {
            for (auto& [name, child] : _children) 
            {
                if (child->_state.initialized)
                    child->_Destroy();
            }
            
            OnDestroy();
            return false;
        }

        _state.initialized = true;
        return true;
    }

    void SceneNode::_Destroy()
    {
        LoggerContext ctx("SceneNode(" + _name + ")", "_Destroy");

        if (!_state.initialized)
        {
            Logger::Warning("Node is not initialized! Aborting...");
            return;
        }

        for (auto& [name, child] : _children)
            child->_Destroy();

        OnDestroy();
        
        _state.initialized = false;
    }

    void SceneNode::_Update()
    {
        if (!_state.enabled || !_state.initialized) 
            return;

        LoggerContext ctx("SceneNode(" + _name + ")", "_Update");

        OnUpdate();

        for (auto& [name, child] : _children)
            child->_Update();
    }

    void SceneNode::_Render()
    {
        if (!_state.enabled || !_state.initialized) 
            return;

        LoggerContext ctx("SceneNode(" + _name + ")", "_Render");

        OnRender();

        for (auto& [name, child] : _children)
            child->_Render();
    }

    void SceneNode::_SetEngine(Engine* engine)
    {
        this->engine = engine;

        for (auto& [name, child] : _children)
            child->_SetEngine(engine);
    }
}