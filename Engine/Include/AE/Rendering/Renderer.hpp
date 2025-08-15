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
    class ShaderManager;
    class Renderer
    {
    public:
    
        Renderer(LightManager* lightMgr, ShaderManager* shaderMgr);
        ~Renderer();
    
        void SubmitMesh(Mesh* mesh, Material* material = nullptr, const glm::mat4& transform = glm::mat4(1.0f));
        void SubmitModel(Model* model, const glm::mat4& transform = glm::mat4(1.0f));
        void SubmitModelNode(ModelNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f));
        void SubmitSkybox(Skybox* skybox);

        RenderMode GetRenderMode() const;
        void SetRenderMode(RenderMode mode);
    
        SPtr<Camera> GetCamera() const;
        void SetCamera(SPtr<Camera> camera);
        
        bool IsInitialized() const;
    
    private:
    
        struct RenderBatch
        {
            const Material* material;
    
            struct InstanceData
            {
                Mesh* mesh;
                glm::mat4 transform;
            };
    
            std::vector<InstanceData> instances;
        };

        LightManager* _lightMgr = nullptr;
        ShaderManager* _shaderMgr = nullptr;
    
        RenderMode _renderMode = RenderMode::Default;
        
        std::vector<RenderBatch> _opaqueBatches;
        std::vector<RenderBatch> _transparentBatches;
        Skybox* _submittedSkybox = nullptr;
    
        SPtr<Camera> _camera;
    
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
