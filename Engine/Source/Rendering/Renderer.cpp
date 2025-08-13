#include "Rendering/Renderer.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/Material.hpp"
#include "Resources/Managers.hpp"
#include "Resources/Shader.hpp"
#include "Resources/Cubemap.hpp"
#include "Resources/Model.hpp"
#include "Resources/Cubemap.hpp"
#include "Lighting/Manager.hpp"
#include "World/Skybox.hpp"
#include "Core/EngineSettings.hpp"
#include "Core/Logger.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace AE
{
    Renderer::Renderer(LightManager* lightMgr, ShaderManager* shaderMgr)
    {
        assert(lightMgr != nullptr);
        assert(shaderMgr != nullptr);

        _lightMgr = lightMgr;
        _shaderMgr = shaderMgr;
    }
    
    Renderer::~Renderer()
    {
        if (_state.initialized)
        {
            LoggerContext ctx("Renderer", "~Renderer");
            Logger::Warning("Destructor called before shutdown!");
            _Shutdown();
        }
    }
    
    void Renderer::SubmitMesh(Mesh* mesh, Material* material, const glm::mat4& transform)
    {
        if (!mesh) return;

        if (_camera)
        {
            const auto& frustum = _camera->GetFrustum();
            AABB worldAABB = mesh->GetAABB().Transform(transform);
            if (!frustum.Intersects(worldAABB))
                return;
        }
    
        const Material* mat = material ? material : Material::GetDefault();
        if (mat->GetShader() == nullptr)
            return;

        auto& batches = material->IsTransparent() ? _transparentBatches : _opaqueBatches;
    
        for (RenderBatch& batch : batches)
        {
            if (batch.material == mat)
            {
                batch.instances.emplace_back(RenderBatch::InstanceData{mesh, transform});
                return;
            }
        }
    
        batches.emplace_back(RenderBatch{mat, {{mesh, transform}}});
    }

    void Renderer::SubmitModel(Model* model, const glm::mat4& transform)
    {
        if (!model) return;

        SubmitModelNode(model->root.get(), transform);
    }

    void Renderer::SubmitModelNode(ModelNode* node, const glm::mat4& parentTransform)
    {
        if (!node) return;

        glm::mat4 globalTransform = parentTransform * node->GetTransform();

        const auto& meshes = node->GetMeshes();
        const auto& materials = node->GetMaterials();

        for (size_t i = 0; i < node->GetMeshesCount(); ++i)
        {
            Material* material = (i < node->GetMaterialsCount()) ? materials[i].get() : nullptr;
            SubmitMesh(meshes[i].get(), material, globalTransform);
        }

        for (auto child : node->GetChildren())
            SubmitModelNode(child.get(), globalTransform);
    }

    void Renderer::SubmitSkybox(Skybox* skybox)
    {
        _submittedSkybox = skybox;
    }

    RenderMode Renderer::GetRenderMode() const { return _renderMode; }
    void Renderer::SetRenderMode(RenderMode mode) { _renderMode = mode; }
    
    std::shared_ptr<Camera> Renderer::GetCamera() const { return _camera; }
    void Renderer::SetCamera(std::shared_ptr<Camera> camera) { _camera = camera; }
    
    bool Renderer::IsInitialized() const { return _state.initialized; }
    
    bool Renderer::_Initialize()
    {
        LoggerContext ctx("Renderer", "_Initialize");
    
        Logger::Info("Initializing renderer...");
    
        if (_state.initialized)
        {
            Logger::Error("Renderer is already initialized! Aborting...");
            return false;
        }
    
        _state.initialized = true;
    
        Logger::Info("Renderer initialized!");
    
        return true;
    }
    
    void Renderer::_Shutdown()
    {
        LoggerContext ctx("Renderer", "_Shutdown");
    
        Logger::Info("Shutting down renderer...");
    
        if (!_state.initialized)
        {
            Logger::Error("Renderer is not initialized! Aborting...");
            return;
        }
        
        _camera.reset();

        _opaqueBatches.clear();
        _transparentBatches.clear();
        _submittedSkybox = nullptr;
    
        _state.initialized = false;
    
        Logger::Info("Renderer shutdown!");
    }
    
    void Renderer::_RenderBatch(const RenderBatch& batch)
    {
        if (!batch.material) return;
    
        Shader* shader = batch.material->GetShader().get();
        shader->Bind();
        
        if (_camera) {
            shader->SetMat4("u_ProjectionMatrix", _camera->GetProjectionMatrix());
            shader->SetMat4("u_ViewMatrix", _camera->GetViewMatrix());
            shader->SetVec3("u_CameraPos", _camera->transform.GetWorldPosition());
        } else {
            shader->SetMat4("u_ProjectionMatrix", glm::mat4(1.0f));
            shader->SetMat4("u_ViewMatrix", glm::mat4(1.0f));
            shader->SetVec3("u_CameraPos", glm::vec3(0.0f));
        }

        shader->SetInt("u_RenderMode", static_cast<int>(_renderMode));
        
        if (batch.material)
            batch.material->Apply();
        
        if (_lightMgr)
            _lightMgr->Apply(shader);
    
        for (const auto& instance : batch.instances)
        {
            shader->SetMat4("u_ModelMatrix", instance.transform);
            instance.mesh->Draw();
        }
    }

    void Renderer::_RenderSkybox()
    {
        if (!_submittedSkybox) return;
        
        Shader* shader = _shaderMgr->GetSkyboxShader().get();
        if (!shader) return;
    
        shader->Bind();

        glm::mat4 view, projection;

        if (_camera) {
            view = glm::mat4(glm::mat3(_camera->GetViewMatrix()));
            projection = _camera->GetProjectionMatrix();
        } else {
            view = glm::mat4(glm::mat3(1.0f));
            projection = glm::mat4(1.0f);
        }

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);
        
        _submittedSkybox->cubemap->Bind();

        shader->SetMat4("u_ViewMatrix", view);
        shader->SetMat4("u_ProjectionMatrix", projection);
        shader->SetInt("u_Cubemap", 0);

        _submittedSkybox->mesh->Draw();

        _submittedSkybox->cubemap->Unbind();

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    
    void Renderer::_RenderOpaqueBatches()
    {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);

        for (const auto& batch : _opaqueBatches)
            _RenderBatch(batch);
    }

    void Renderer::_RenderTransparentBatches()
    {
        glm::vec3 camPos;
        if (_camera)
        {
            camPos = _camera->transform.GetWorldPosition();
        } else {
            camPos = glm::vec3(0.0f);
        }

        std::sort(_transparentBatches.begin(), _transparentBatches.end(),
            [&camPos](const RenderBatch& a, const RenderBatch& b)
            {
                float maxDistSqA = 0.0f;
                for (const auto& inst : a.instances)
                {
                    glm::vec3 pos = inst.transform[3];
                    float distSq = glm::length2(pos - camPos);
                    if (distSq > maxDistSqA) maxDistSqA = distSq;
                }

                float maxDistSqB = 0.0f;
                for (const auto& inst : b.instances)
                {
                    glm::vec3 pos = inst.transform[3];
                    float distSq = glm::length2(pos - camPos);
                    if (distSq > maxDistSqB) maxDistSqB = distSq;
                }

                return maxDistSqA > maxDistSqB;
            });

        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (const auto& batch : _transparentBatches)
        {
            _RenderBatch(batch);
        }

        glDepthMask(GL_TRUE);
    }

    void Renderer::_PrepareFrame()
    {
        EngineSettings& settings = EngineSettings::Get();

        if (settings.renderer.enableDepthTest) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        
        if (settings.renderer.enableFaceCulling) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        } else {
            glDisable(GL_CULL_FACE);
        }

        const Color& clearColor = settings.renderer.clearColor;

        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        _opaqueBatches.clear();
        _opaqueBatches.reserve(128);

        _transparentBatches.clear();
        _transparentBatches.reserve(128);

        _submittedSkybox = nullptr;
    }
    
    void Renderer::_RenderFrame()
    {
        switch (_renderMode)
        {
            case RenderMode::Wireframe:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;

            default:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
        }
        
        _RenderOpaqueBatches();
        _RenderSkybox();
        _RenderTransparentBatches();
    }
}
