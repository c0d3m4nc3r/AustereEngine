#include "Game/Nodes.hpp"

#include <AE/Core/Engine.hpp>
#include <AE/Rendering/Renderer.hpp>
#include <AE/Resources/Shader.hpp>
#include <AE/Resources/Model.hpp>
#include <AE/Resources/Managers.hpp>

bool TestNode::OnInitialize()
{
    AE::ShaderManager* shaderMgr = engine->GetShaderManager();
    AE::ModelManager* modelMgr = engine->GetModelManager();
    
    mainShader = shaderMgr->Get("Main");
    if (!mainShader) return false;

    testModel = modelMgr->Load("SponzaAtrium",
        "Assets/Models/SponzaAtrium3.glb"
    );

    if (!testModel) return false;
    
    return true;
}

void TestNode::OnDestroy()
{
    mainShader.reset();
    testModel.reset();
}

void TestNode::OnRender()
{
    AE::Renderer* renderer = engine->GetRenderer();
    
    renderer->SubmitModel(testModel.get(), mainShader.get(), transform.GetWorldMatrix());
}

void TestNode::OnUpdate()
{
    
}