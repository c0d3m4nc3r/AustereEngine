#pragma once

#include "PCH.hpp"

namespace AE
{
    class Camera;
    class Shader;
    class Material;
    class Mesh;
    class Model;
    class ModelNode;
    class Skybox;

    enum class RenderMode
    {
        Default,
        Wireframe
    };
    
    class LightManager;
    class Renderer
    {
    public:
    
        Renderer(LightManager* lightMgr = nullptr);
        ~Renderer();
    
        void SubmitMesh(Mesh* mesh, Shader* shader, Material* material = nullptr, const glm::mat4& transform = glm::mat4(1.0f));
        void SubmitModel(Model* model, Shader* shader, const glm::mat4& transform = glm::mat4(1.0f));
        void SubmitModelNode(ModelNode* node, Shader* shader, const glm::mat4& parentTransform = glm::mat4(1.0f));

        RenderMode GetRenderMode() const;
        void SetRenderMode(RenderMode mode);
    
        std::shared_ptr<Camera> GetCamera() const;
        void SetCamera(std::shared_ptr<Camera> camera);
   
        std::shared_ptr<Skybox> GetSkybox() const;
        void SetSkybox(std::shared_ptr<Skybox> skybox);
        void SetSkyboxShader(std::shared_ptr<Shader> shader);
        
        bool IsInitialized() const;
    
    private:
    
        struct RenderBatch
        {
            Shader* shader;
            const Material* material;
    
            struct InstanceData
            {
                Mesh* mesh;
                glm::mat4 transform;
            };
    
            std::vector<InstanceData> instances;
        };

        LightManager* _lightMgr;
    
        RenderMode _renderMode = RenderMode::Default;
        
        std::vector<RenderBatch> _opaqueBatches;
        std::vector<RenderBatch> _transparentBatches;
    
        std::shared_ptr<Camera> _camera;

        std::shared_ptr<Skybox> _skybox;
        std::shared_ptr<Shader> _skyboxShader;
    
        struct RendererState
        {
            bool initialized = false;
        } _state;
    
        bool _Initialize();
        void _Shutdown();
        
        void _RenderBatch(const RenderBatch& batch);
        void _RenderSkybox();
        
        void _RenderOpaqueBatches();
        void _RenderTransparentBatches();
    
        void _PrepareFrame();
        void _RenderFrame();
    
        friend class Engine;
    };
}
