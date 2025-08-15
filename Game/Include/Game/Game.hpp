#pragma once

#include <AE/Core/Application.hpp>

namespace AE
{
    class Scene;
}

class TestCamera;
class AustereGame : public AE::Application
{
public:

    AustereGame(const AE::ApplicationInfo& info);
    ~AustereGame();

    bool OnInitialize() override;
    void OnShutdown() override;
    void OnUpdate() override;
    void OnRender() override;

private:

    struct GameScenes {
        AE::SPtr<AE::Scene> test;
    } _scenes;
    
};
