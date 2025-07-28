#pragma once

#include "PCH.hpp"

namespace AE
{
    class Engine;
    class Scene;
    class SceneManager
    {
    public:

        SceneManager(Engine* engine);

        bool AddScene(std::shared_ptr<Scene> scene);
        bool RemoveScene(const std::string& name);
        bool RemoveScene(const std::shared_ptr<Scene>& scene);

        std::shared_ptr<Scene> GetScene(const std::string& name) const;
        int GetScenesCount() const;

        std::shared_ptr<Scene> GetActiveScene() const;
        void SetActiveScene(const std::string& name);

        bool HasScene(const std::string& name);
        bool HasScene(std::shared_ptr<Scene> scene);

    private:

        Engine* _engine = nullptr;

        std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;
        std::weak_ptr<Scene> _activeScene;

        void _Update();
        void _Render();

        friend class Engine;
    };
}
