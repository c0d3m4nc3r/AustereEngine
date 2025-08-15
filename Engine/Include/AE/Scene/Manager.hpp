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

        bool AddScene(SPtr<Scene> scene);
        bool RemoveScene(const std::string& name);
        bool RemoveScene(const SPtr<Scene>& scene);

        SPtr<Scene> GetScene(const std::string& name) const;
        int GetScenesCount() const;

        SPtr<Scene> GetActiveScene() const;
        void SetActiveScene(const std::string& name);

        bool HasScene(const std::string& name);
        bool HasScene(SPtr<Scene> scene);

    private:

        Engine* _engine = nullptr;

        std::unordered_map<std::string, SPtr<Scene>> _scenes;
        WPtr<Scene> _activeScene;

        void _Update();
        void _Render();

        friend class Engine;
    };
}
