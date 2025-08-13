#include "Game/Nodes.hpp"

#include <AE/Core/Engine.hpp>
#include <AE/Rendering/Renderer.hpp>
#include <AE/Resources/Shader.hpp>
#include <AE/Resources/Model.hpp>
#include <AE/Resources/Managers.hpp>

bool TestNode::OnInitialize()
{
    AE::ModelManager* modelMgr = engine->GetModelManager();

    _testModel = modelMgr->Load("SponzaAtrium",
        "Assets/Models/SponzaAtrium3.glb"
    );

    if (!_testModel) return false;
    
    return true;
}

void TestNode::OnDestroy()
{
    _testModel.reset();
}

void TestNode::OnRender()
{
    AE::Renderer* renderer = engine->GetRenderer();
    
    renderer->SubmitModel(_testModel.get(), transform.GetWorldMatrix());
}

void TestNode::OnUpdate()
{
    
}