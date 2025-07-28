#pragma once

#include "Resources/Manager.hpp"

#include <assimp/scene.h>

namespace AE
{
    class Shader;
    class ShaderManager : public ResourceManager<Shader>
    {
    public:
        
        ShaderManager();

        std::shared_ptr<Shader> Load(const std::string& name,
            const std::string& vertexPath,
            const std::string& fragmentPath
        );

    };

    class Texture;
    class TextureManager : public ResourceManager<Texture>
    {
    public:

        TextureManager();

        std::shared_ptr<Texture> Load(const std::string& name,
            const std::string& path
        );

        std::shared_ptr<Texture> Load(const std::string& name,
            unsigned char* data, size_t size,
            bool compressed = false,
            int width = 0, int height = 0,
            int channels = 4
        );

    };

    class Cubemap;
    class CubemapManager : public ResourceManager<Cubemap>
    {
    public:

        CubemapManager();

        std::shared_ptr<Cubemap> Load(const std::string& name,
            const std::array<std::string, 6>& paths
        );

    };

    class Material;
    class Mesh;
    class Model;
    class ModelNode;
    class ModelManager : public ResourceManager<Model>
    {
    public:

        ModelManager(TextureManager* textureMgr);

        std::shared_ptr<Model> Load(const std::string& name,
            const std::string& path
        );
    
    private:

        // NOTE: Not thread-safe! Fix if parallel loading is needed.
        std::string _directory = ".";
        std::string _currentModelName;

        TextureManager* _textureMgr;

        void _ProcessNode(aiNode* node, const aiScene* scene, const std::shared_ptr<ModelNode>& parent);
        std::shared_ptr<Mesh> _ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::shared_ptr<Material> _ProcessMaterial(aiMaterial* aiMat, const aiScene* scene);
        std::shared_ptr<Texture> _LoadTexture(aiMaterial* aiMat, aiTextureType type, const aiScene* scene);
        glm::mat4 _ConvertMatrix(const aiMatrix4x4& aiMat);

    };
}