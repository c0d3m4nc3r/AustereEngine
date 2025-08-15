#include "Game/Nodes.hpp"

#include <AE/Core/Engine.hpp>
#include <AE/Rendering/Renderer.hpp>
#include <AE/Resources/Managers.hpp>
#include <AE/Lighting/Manager.hpp>

bool TestSceneRoot::OnInitialize()
{
    AE::CubemapManager* cubemapMgr = engine->GetCubemapManager();

    auto cubemap = cubemapMgr->Load("Skybox", {
        "Assets/Skyboxes/Clouds_East.bmp",   // +X (right)
        "Assets/Skyboxes/Clouds_West.bmp",   // -X (left)
        "Assets/Skyboxes/Clouds_Up.bmp",     // +Y (top)
        "Assets/Skyboxes/Clouds_Down.bmp",   // -Y (bottom)
        "Assets/Skyboxes/Clouds_North.bmp",  // +Z (front)
        "Assets/Skyboxes/Clouds_South.bmp"   // -Z (back)
    });

    if (!cubemap)
    {
        AE::Logger::Error("Failed to load skybox cubemap!");
        return false;
    }

    _skybox = AE::MakeUPtr<AE::Skybox>(cubemap);

    _cameraNode = AE::MakeSPtr<CameraNode>();
    AddChild(_cameraNode);
    AddChild(AE::MakeSPtr<TestNode>());

    AE::LightManager* lightMgr = engine->GetLightManager();
    lightMgr->AddLight("Sun", AE::MakeUPtr<AE::DirectionalLight>(
        AE::Color::White, 1.0f, glm::normalize(glm::vec3(-0.5f, -1.0f, -0.5f))
    ));

    return true;
}

void TestSceneRoot::OnDestroy()
{
    _skybox.reset();
    _cameraNode.reset();

    AE::LightManager* lightMgr = engine->GetLightManager();
    lightMgr->RemoveLight("Sun");
}

void TestSceneRoot::OnRender()
{
    AE::Renderer* renderer = engine->GetRenderer();
    renderer->SetCamera(_cameraNode->camera);
    renderer->SubmitSkybox(_skybox.get());
}
