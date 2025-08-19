#pragma once

#include "Resources/ManagerBase.hpp"

#include <assimp/scene.h>

namespace AE
{
    class Shader;
    class ShaderManager : public ResourceManagerBase<Shader>
    {
    public:
        
        ShaderManager();

        SPtr<Shader> Load(const std::string& name,
            const std::string& vertexPath,
            const std::string& fragmentPath
        );

        SPtr<Shader> LoadFromSource(const std::string& name,
            const std::string& vertexSrc,
            const std::string& fragmentSrc
        );

        SPtr<Shader> GetBuiltinShader(const std::string& name);

        SPtr<Shader> GetStandardShader() const;
        SPtr<Shader> GetSkyboxShader() const;

        void SetStandardShader(SPtr<Shader> shader);
        void SetSkyboxShader(SPtr<Shader> shader);

    private:

        SPtr<Shader> _standardShader;
        SPtr<Shader> _skyboxShader;

        std::unordered_map<std::string, SPtr<Shader>> _builtinShaders;

        bool _LoadBuiltinShaders();

        friend class Engine;
    };

    class Texture;
    class TextureManager : public ResourceManagerBase<Texture>
    {
    public:

        TextureManager();

        SPtr<Texture> Load(const std::string& name,
            const std::string& path
        );

        SPtr<Texture> Load(const std::string& name,
            unsigned char* data, size_t size,
            bool compressed = false,
            int width = 0, int height = 0,
            int channels = 4
        );

    };

    class Cubemap;
    class CubemapManager : public ResourceManagerBase<Cubemap>
    {
    public:

        CubemapManager();

        SPtr<Cubemap> Load(const std::string& name,
            const std::array<std::string, 6>& paths
        );

    };

    class Material;
    class Mesh;
    class Model;
    class ModelNode;
    class ModelManager : public ResourceManagerBase<Model>
    {
    public:

        ModelManager(TextureManager* textureMgr, ShaderManager* shaderMgr);

        SPtr<Model> Load(const std::string& name,
            const std::string& path
        );
    
    private:

        // NOTE: Not thread-safe! Fix if parallel loading is needed.
        std::string _directory = ".";
        std::string _currentModelName;

        TextureManager* _textureMgr;
        ShaderManager* _shaderMgr;

        void _ProcessNode(aiNode* node, const aiScene* scene, const SPtr<ModelNode>& parent);
        SPtr<Mesh> _ProcessMesh(aiMesh* mesh, const aiScene* scene);
        SPtr<Material> _ProcessMaterial(aiMaterial* aiMat, const aiScene* scene);
        SPtr<Texture> _LoadTexture(aiMaterial* aiMat, aiTextureType type, const aiScene* scene);
        glm::mat4 _ConvertMatrix(const aiMatrix4x4& aiMat);

    };
}