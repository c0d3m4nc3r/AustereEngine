#include "Game/Game.hpp"
#include "Game/Nodes.hpp"

#include <AE/Core/Engine.hpp>
#include <AE/Platform/Input.hpp>
#include <AE/Platform/Window.hpp>
#include <AE/Rendering/Renderer.hpp>
#include <AE/Scene/Manager.hpp>
#include <AE/Scene/Scene.hpp>


AustereGame::AustereGame(const AE::ApplicationInfo& info)
    : AE::Application(info) {}

AustereGame::~AustereGame() {}

bool AustereGame::OnInitialize()
{
    _scenes.test = AE::MakeSPtr<AE::Scene>("TestScene", AE::MakeSPtr<TestSceneRoot>());

    AE::SceneManager* sceneMgr = engine->GetSceneManager();
    sceneMgr->AddScene(_scenes.test);
    sceneMgr->SetActiveScene("TestScene");
    
    return true;
}

void AustereGame::OnShutdown()
{
    _scenes.test.reset();
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
}

void AustereGame::OnRender() {}
