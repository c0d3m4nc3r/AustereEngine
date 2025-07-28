#include "Resources/Managers.hpp"
#include "Resources/Model.hpp"
#include "Resources/Texture.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Material.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <filesystem>

namespace AE
{
    namespace fs = std::filesystem;
    
    ModelManager::ModelManager(TextureManager* textureMgr)
        : ResourceManager("Model"), _textureMgr(textureMgr) {}

    std::shared_ptr<Model> ModelManager::Load(const std::string& name,
        const std::string& path)
    {
        LoggerContext ctx("ModelManager", "Load");

        if (Has(name))
        {
            Logger::Error("Model with name '{}' already exists!", name);
            return Get(name);
        }
        
        Logger::Info("Loading model '{}' from \'{}\'", name, path);

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, 
            aiProcess_Triangulate | 
            aiProcess_FlipUVs | 
            aiProcess_GenSmoothNormals | 
            aiProcess_GenBoundingBoxes | 
            aiProcess_CalcTangentSpace | 
            aiProcess_OptimizeMeshes
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Logger::Error("Assimp error: {}", importer.GetErrorString());
            return nullptr;
        }

        fs::path p(path);
        _directory = p.has_parent_path() ? p.parent_path().string() : ".";

        auto model = std::make_shared<Model>();

        model->root = std::make_shared<ModelNode>(
            scene->mRootNode->mName.C_Str(),
            _ConvertMatrix(scene->mRootNode->mTransformation)
        );

        _currentModelName = name;

        _ProcessNode(scene->mRootNode, scene, model->root);

        Add(name, model);

        Logger::Info("Model '{}' loaded successfully!", name);

        return model;
    }

    void ModelManager::_ProcessNode(aiNode* node, const aiScene* scene, const std::shared_ptr<ModelNode>& parent)
    {
        LoggerContext ctx("ModelManager", "_ProcessNode");

        Logger::Debug("Processing node '{}' with {} meshes and {} children...", node->mName.C_Str(), node->mNumMeshes, node->mNumChildren);

        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            
            auto processedMesh = _ProcessMesh(mesh, scene);
            auto processedMaterial = _ProcessMaterial(scene->mMaterials[mesh->mMaterialIndex], scene);

            parent->AddMesh(std::move(processedMesh));
            parent->AddMaterial(std::move(processedMaterial));
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            auto childNode = std::make_shared<ModelNode>(
                node->mChildren[i]->mName.C_Str(),
                _ConvertMatrix(node->mChildren[i]->mTransformation)
            );
            
            parent->AddChild(childNode);

            _ProcessNode(node->mChildren[i], scene, childNode);
        }
    }

    std::shared_ptr<Mesh> ModelManager::_ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        LoggerContext ctx("ModelManager", "_ProcessMesh");

        Logger::Debug("Processing mesh '{}' with {} vertices and {} faces...", mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);

        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;

        vertices.reserve(mesh->mNumVertices);
        indices.reserve(mesh->mNumFaces * 3);
        normals.reserve(mesh->mNumVertices);
        texCoords.reserve(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            vertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
            normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
            
            if (mesh->mTextureCoords[0]) {
                texCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
            } else {
                texCoords.push_back(glm::vec2(0.0f, 0.0f));
            }
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        auto outMesh = std::make_shared<Mesh>(vertices, indices, normals, texCoords);
        
        glm::vec3 aabbMin = {mesh->mAABB.mMin[0], mesh->mAABB.mMin[1], mesh->mAABB.mMin[2]};
        glm::vec3 aabbMax = {mesh->mAABB.mMax[0], mesh->mAABB.mMax[1], mesh->mAABB.mMax[2]};

        outMesh->SetAABB({aabbMin, aabbMax});
    
        return outMesh;
    }

    static const char* TextureTypeToString(aiTextureType type)
    {
        switch (type)
        {
            case aiTextureType_DIFFUSE: return "DIFFUSE";
            case aiTextureType_SPECULAR: return "SPECULAR";
            case aiTextureType_AMBIENT: return "AMBIENT";
            case aiTextureType_EMISSIVE: return "EMISSIVE";
            case aiTextureType_HEIGHT: return "HEIGHT";
            case aiTextureType_NORMALS: return "NORMALS";
            case aiTextureType_SHININESS: return "SHININESS";
            case aiTextureType_OPACITY: return "OPACITY";
            case aiTextureType_DISPLACEMENT: return "DISPLACEMENT";
            case aiTextureType_LIGHTMAP: return "LIGHTMAP";
            case aiTextureType_REFLECTION: return "REFLECTION";
            default: return "UNKNOWN";
        }
    }

    std::shared_ptr<Material> ModelManager::_ProcessMaterial(aiMaterial* aiMat, const aiScene* scene)
    {
        aiColor3D color;
        float shininess = 32.0f;
        float opacity = 1.0f;

        if (aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color) != AI_SUCCESS)
            color = aiColor3D(0.2f, 0.2f, 0.2f);
        Color ambient(color.r, color.g, color.b, 1.0f);

        if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color) != AI_SUCCESS)
            color = aiColor3D(0.8f, 0.8f, 0.8f);
        Color diffuse(color.r, color.g, color.b, 1.0f);

        if (aiMat->Get(AI_MATKEY_COLOR_SPECULAR, color) != AI_SUCCESS)
            color = aiColor3D(1.0f, 1.0f, 1.0f);
        Color specular(color.r, color.g, color.b, 1.0f);

        if (aiMat->Get(AI_MATKEY_SHININESS, shininess) != AI_SUCCESS || shininess < 8.0f)
        {
            shininess = 32.0f;
        }

        auto material = std::make_shared<Material>(ambient, diffuse, specular, shininess);

        for (int type = aiTextureType_NONE + 1; type <= aiTextureType_REFLECTION; ++type)
        {
            unsigned int count = aiMat->GetTextureCount(static_cast<aiTextureType>(type));
            if (count > 0)
            {
                Logger::Debug("Material '{}' has {} texture(s) of type {}", aiMat->GetName().C_Str(), count, TextureTypeToString(static_cast<aiTextureType>(type)));
            }
        }

        auto LoadAndSetTexture = [&](aiTextureType type, auto setter)
        {
            if (aiMat->GetTextureCount(type) > 0)
            {
                std::shared_ptr<Texture> tex = _LoadTexture(aiMat, type, scene);
                if (tex)
                {
                    (material.get()->*setter)(tex);
                }
            }
        };

        LoadAndSetTexture(aiTextureType_DIFFUSE, &Material::SetDiffuseTexture);
        LoadAndSetTexture(aiTextureType_SPECULAR, &Material::SetSpecularTexture);
        LoadAndSetTexture(aiTextureType_EMISSIVE, &Material::SetEmissiveTexture);
        LoadAndSetTexture(aiTextureType_NORMALS, &Material::SetNormalTexture);
        LoadAndSetTexture(aiTextureType_OPACITY, &Material::SetOpacityTexture);

        return material;
    }

    std::shared_ptr<Texture> ModelManager::_LoadTexture(aiMaterial* aiMat, aiTextureType type, const aiScene* scene)
    {
        LoggerContext ctx("ModelManager", "_LoadTexture");

        if (aiMat->GetTextureCount(type) == 0)
            return Texture::GetDefault();

        aiString path;
        if (aiMat->GetTexture(type, 0, &path) != AI_SUCCESS)
        {
            Logger::Error("Failed to get texture path from material {}!", aiMat->GetName().C_Str());
            return Texture::GetDefault();
        }

        std::string pathStr = path.C_Str();

        // Embedded texture
        if (pathStr[0] == '*')
        {
            std::string texKey = _currentModelName + ":" + pathStr;

            if (_textureMgr->Has(texKey))
                return _textureMgr->Get(texKey);

            const aiTexture* embeddedTex = scene->GetEmbeddedTexture(pathStr.c_str());
            if (!embeddedTex || !embeddedTex->pcData)
            {
                Logger::Error("Invalid embedded texture: \"{}\"", pathStr);
                return Texture::GetDefault();
            }

            if (embeddedTex->mHeight == 0)
            {
                // Compressed (e.g., PNG in-memory)
                return _textureMgr->Load(texKey,
                    reinterpret_cast<unsigned char*>(embeddedTex->pcData),
                    embeddedTex->mWidth,
                    true
                );
            }
            else
            {
                // Raw pixel data (e.g., RGBA)
                size_t size = embeddedTex->mWidth * embeddedTex->mHeight * sizeof(aiTexel);
                return _textureMgr->Load(texKey,
                    reinterpret_cast<unsigned char*>(embeddedTex->pcData),
                    size,
                    false,
                    embeddedTex->mWidth,
                    embeddedTex->mHeight,
                    4
                );
            }
        }
        else
        {
            std::string fullPath = _directory + "/" + pathStr;
            std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

            if (_textureMgr->Has(fullPath))
                return _textureMgr->Get(fullPath);

            return _textureMgr->Load(fullPath, fullPath);
        }
    }

    glm::mat4 ModelManager::_ConvertMatrix(const aiMatrix4x4& aiMat)
    {
        return glm::mat4(
            aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
            aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
            aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
            aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
        );
    }
}
