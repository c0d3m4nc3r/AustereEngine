#pragma once

#include <AE/Core/Application.hpp>
#include <AE/Rendering/Material.hpp>

namespace AE
{
    class Texture;
    class Cubemap;
    class Skybox;
    class Shader;
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
    
    struct GameShaders {
        std::shared_ptr<AE::Shader> main;
        std::shared_ptr<AE::Shader> skybox;
    } _shaders;

    struct GameScenes {
        std::shared_ptr<AE::Scene> test;
    } _scenes;

    std::shared_ptr<AE::Skybox> _testSkybox;
    std::shared_ptr<AE::Cubemap> _skyboxCubemap;
    std::shared_ptr<TestCamera> _testCamera;


    bool _LoadResources();
};
