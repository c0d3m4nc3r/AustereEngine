#pragma once

#include "PCH.hpp"

#include "Math/Transform.hpp"

namespace AE
{
    class Engine;
    class SceneNode : public std::enable_shared_from_this<SceneNode>
    {
    public:
        
        SceneNode(const std::string& name);
        virtual ~SceneNode();

        bool AddChild(std::shared_ptr<SceneNode> node);
        bool RemoveChild(const std::string& name);
        bool RemoveChild(const std::shared_ptr<SceneNode>& node);

        const std::string& GetName() const;

        Transform& GetTransform();
        const Transform& GetTransform() const;
        void SetTransform(const Transform& transform);

        std::shared_ptr<SceneNode> GetChild(const std::string& name) const;
        std::size_t GetChildrenCount() const;

        std::shared_ptr<SceneNode> GetParent() const;
        void SetParent(std::shared_ptr<SceneNode> node);
        bool HasParent() const;
        
        bool HasChildren() const;
        bool HasChild(const std::string& name) const;
        bool HasChild(const std::shared_ptr<SceneNode>& node) const;

        bool IsInitialized() const;
        bool IsEnabled() const;
        void SetEnabled(bool enabled);

    protected:

        Transform transform;

        Engine* engine = nullptr;

        virtual bool OnInitialize() { return true; }
        virtual void OnDestroy() {}
        virtual void OnUpdate() {}
        virtual void OnRender() {}

    private:

        const std::string _name;
        std::unordered_map<std::string, std::shared_ptr<SceneNode>> _children;
        std::weak_ptr<SceneNode> _parent;

        struct SceneNodeState
        {
            bool initialized = false;
            bool enabled = true;
        } _state;

        bool _Initialize();
        void _Destroy();
        void _Update();
        void _Render();

        void _SetEngine(Engine* engine);

        friend class Scene;
    };
}
