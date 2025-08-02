#include "Game/Game.hpp"
#include "Game/Nodes.hpp"

#include <AE/Core/Engine.hpp>
#include <AE/Core/Logger.hpp>
#include <AE/Platform/Input.hpp>
#include <AE/Platform/Window.hpp>
#include <AE/Lighting/Manager.hpp>
#include <AE/Lighting/Sources.hpp>
#include <AE/Rendering/Camera.hpp>
#include <AE/Rendering/Renderer.hpp>
#include <AE/Resources/Shader.hpp>
#include <AE/Resources/Texture.hpp>
#include <AE/Resources/Managers.hpp>
#include <AE/Scene/Manager.hpp>
#include <AE/Scene/Scene.hpp>
#include <AE/World/Skybox.hpp>

AustereGame::AustereGame(const AE::ApplicationInfo& info)
    : AE::Application(info) {}

AustereGame::~AustereGame() {}

bool AustereGame::OnInitialize()
{
    if (!_LoadResources())
    {
        AE::Logger::Error("Failed to load resources!");
        return false;
    }

    // Create test scene
    _scenes.test = std::make_shared<AE::Scene>("TestScene");
    const auto root = _scenes.test->GetRoot();
    
    // Create camera and test nodes and add them to the test scene 
    auto cameraNode = std::make_shared<CameraNode>();
    root->AddChild(cameraNode);

    root->AddChild(std::make_shared<TestNode>());

    // Add and activate test scene
    AE::SceneManager* sceneMgr = engine->GetSceneManager();
    sceneMgr->AddScene(_scenes.test);
    sceneMgr->SetActiveScene("TestScene");

    // Create skybox
    _testSkybox = std::make_shared<AE::Skybox>(_skyboxCubemap);

    // Add light sources
    AE::LightManager* lightMgr = engine->GetLightManager();
    lightMgr->AddLight(std::make_unique<AE::DirectionalLight>(
        AE::Color::White, 1.0f, glm::normalize(glm::vec3(-0.5f, -1.0f, -0.5f))
    ));

    // Setup renderer
    AE::Renderer* renderer = engine->GetRenderer();
    renderer->SetRenderMode(AE::RenderMode::Default);
    renderer->SetCamera(cameraNode->camera);
    renderer->SetSkybox(_testSkybox);
    renderer->SetSkyboxShader(_shaders.skybox);

    // AE::Window* window = engine->GetWindow();
    // window->SetVSync(false);
    
    return true;
}

void AustereGame::OnShutdown()
{
    _shaders.main.reset();
    _shaders.skybox.reset();
    _scenes.test.reset();
    _skyboxCubemap.reset();
    _testSkybox.reset();
}

void AustereGame::OnUpdate()
{
    AE::InputManager* inputMgr = engine->GetInputManager();
    AE::Keyboard* keyboard = inputMgr->GetKeyboard();
    AE::Renderer* renderer = engine->GetRenderer();

    if (keyboard->IsKeyPressed(AE::Key::Escape)) {
        engine->Stop();
    } else if (keyboard->IsKeyPressed(AE::Key::F1)) {
        renderer->SetRenderMode(AE::RenderMode::Default);
    } else if (keyboard->IsKeyPressed(AE::Key::F2)) {
        renderer->SetRenderMode(AE::RenderMode::Wireframe);
    }

    AE::Window* window = engine->GetWindow();
    window->SetTitle(GetName() + " v" + GetVersion() + " | FPS: " + std::to_string(engine->GetFPS()));

    // _testCamera->SetAspectRatio(window->GetAspectRatio());
}

void AustereGame::OnRender()
{

}

bool AustereGame::_LoadResources()
{
    AE::Logger::Info("Loading resources...");

    AE::ShaderManager* shaderMgr = engine->GetShaderManager();
    AE::CubemapManager* cubemapMgr = engine->GetCubemapManager();

    // Load main shader
    _shaders.main = shaderMgr->Load("Main",
        "Assets/Shaders/Main.vert",
        "Assets/Shaders/Main.frag"
    );

    if (!_shaders.main)
    {
        AE::Logger::Error("Failed to load main shader!");
        return false;
    }

    // Load skybox shader
    _shaders.skybox = shaderMgr->Load("Skybox",
        "Assets/Shaders/Skybox.vert",
        "Assets/Shaders/Skybox.frag"
    );

    if (!_shaders.skybox)
    {
        AE::Logger::Error("Failed to load skybox shader!");
        return false;
    }

    // Load skybox cubemap
    _skyboxCubemap = cubemapMgr->Load("Skybox", {
        "Assets/Skyboxes/Clouds_East.bmp",   // +X (right)
        "Assets/Skyboxes/Clouds_West.bmp",   // -X (left)
        "Assets/Skyboxes/Clouds_Up.bmp",     // +Y (top)
        "Assets/Skyboxes/Clouds_Down.bmp",   // -Y (bottom)
        "Assets/Skyboxes/Clouds_North.bmp",  // +Z (front)
        "Assets/Skyboxes/Clouds_South.bmp"   // -Z (back)
    });

    if (!_skyboxCubemap)
    {
        AE::Logger::Error("Failed to load skybox cubemap!");
        return false;
    }

    return true;
}
