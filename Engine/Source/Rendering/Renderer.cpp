#include "Rendering/Renderer.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/Material.hpp"
#include "Resources/Shader.hpp"
#include "Resources/Model.hpp"
#include "Lighting/Manager.hpp"
#include "World/Skybox.hpp"
#include "Core/EngineSettings.hpp"
#include "Core/Logger.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace AE
{
    Renderer::Renderer(LightManager* lightMgr)
    {
        assert(lightMgr != nullptr);
        _lightMgr = lightMgr;
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
    
    void Renderer::SubmitMesh(Mesh* mesh, Shader* shader, Material* material, const glm::mat4& transform)
    {
        if (!mesh || !shader) return;

        if (_camera)
        {
            const auto& frustum = _camera->GetFrustum();
            AABB worldAABB = mesh->GetAABB().Transform(transform);
            if (!frustum.Intersects(worldAABB))
                return;
        }
    
        const Material* mat = material ? material : Material::GetDefault();

        auto& batches = material->IsTransparent() ? _transparentBatches : _opaqueBatches;
    
        for (RenderBatch& batch : batches)
        {
            if (batch.shader == shader && batch.material == mat)
            {
                batch.instances.emplace_back(RenderBatch::InstanceData{mesh, transform});
                return;
            }
        }
    
        batches.emplace_back(RenderBatch{shader, mat, {{mesh, transform}}});
    }

    void Renderer::SubmitModel(Model* model, Shader* shader, const glm::mat4& transform)
    {
        if (!model || !shader) return;

        SubmitModelNode(model->root.get(), shader, transform);
    }

    void Renderer::SubmitModelNode(ModelNode* node, Shader* shader, const glm::mat4& parentTransform)
    {
        if (!node || !shader) return;

        glm::mat4 globalTransform = parentTransform * node->GetTransform();

        const auto& meshes = node->GetMeshes();
        const auto& materials = node->GetMaterials();

        for (size_t i = 0; i < node->GetMeshesCount(); ++i)
        {
            Material* material = (i < node->GetMaterialsCount()) ? materials[i].get() : nullptr;
            SubmitMesh(meshes[i].get(), shader, material, globalTransform);
        }

        for (auto child : node->GetChildren())
            SubmitModelNode(child.get(), shader, globalTransform);
    }

    RenderMode Renderer::GetRenderMode() const { return _renderMode; }
    void Renderer::SetRenderMode(RenderMode mode) { _renderMode = mode; }
    
    std::shared_ptr<Camera> Renderer::GetCamera() const { return _camera; }
    void Renderer::SetCamera(std::shared_ptr<Camera> camera) { _camera = camera; }

    std::shared_ptr<Skybox> Renderer::GetSkybox() const { return _skybox; }
    void Renderer::SetSkybox(std::shared_ptr<Skybox> skybox) { _skybox = skybox; }
    void Renderer::SetSkyboxShader(std::shared_ptr<Shader> shader) { _skyboxShader = shader; }
    
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
    
        _skybox.reset();
        _skyboxShader.reset();
        _camera.reset();

        _opaqueBatches.clear();
        _transparentBatches.clear();
    
        _state.initialized = false;
    
        Logger::Info("Renderer shutdown!");
    }
    
    void Renderer::_RenderBatch(const RenderBatch& batch)
    {
        if (!batch.shader) return;
    
        Shader* shader = batch.shader;
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
            batch.material->Apply(shader);
        
        if (_lightMgr)
            _lightMgr->Apply(shader);
    
        for (const auto& instance : batch.instances)
        {
            shader->SetMat4("u_ModelMatrix", instance.transform);
            instance.mesh->Draw();
        }
    
        shader->Unbind();
    }

    void Renderer::_RenderSkybox()
    {
        if (!_skybox || !_skyboxShader || !_camera) return;
    
        _skyboxShader->Bind();

        glm::mat4 view = glm::mat4(glm::mat3(_camera->GetViewMatrix()));
        glm::mat4 projection = _camera->GetProjectionMatrix();

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);
        
        _skybox->cubemap->Bind();

        _skyboxShader->SetMat4("u_ViewMatrix", view);
        _skyboxShader->SetMat4("u_ProjectionMatrix", projection);
        _skyboxShader->SetInt("u_Cubemap", 0);

        _skybox->mesh->Draw();

        _skybox->cubemap->Unbind();

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        _skyboxShader->Unbind();
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
        const glm::vec3& camPos = _camera->transform.GetWorldPosition();

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
