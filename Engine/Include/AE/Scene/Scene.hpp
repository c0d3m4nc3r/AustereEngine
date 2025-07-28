#pragma once

#include "PCH.hpp"

namespace AE
{
    class Engine;
    class SceneNode;
    class Scene
    {
    public:

        Scene(const std::string& name, std::shared_ptr<SceneNode> root = nullptr);
        ~Scene();

        const std::string& GetName() const;

        std::shared_ptr<SceneNode> GetRoot() const;
        void SetRoot(std::shared_ptr<SceneNode> node);

        bool IsInitialized() const;
        bool IsActive() const;

    private:

        Engine* _engine = nullptr;

        std::string _name;
        std::shared_ptr<SceneNode> _root;

        struct SceneState
        {
            bool initialized = false;
            bool active = false;
        } _state;

        bool _Initialize();
        void _Destroy();
        void _Update();
        void _Render();

        void _SetEngine(Engine* engine);

        friend class SceneManager;
    };
}