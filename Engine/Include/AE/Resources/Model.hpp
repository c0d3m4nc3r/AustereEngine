#pragma once

#include "PCH.hpp"

namespace AE
{
    class Mesh;
    class Material;

    class ModelNode : public std::enable_shared_from_this<ModelNode>
    {
    public:
        
        ModelNode(const std::string& name, const glm::mat4& transform = {1.0f});
        ~ModelNode() = default;

        void AddMesh(const SPtr<Mesh>& mesh);
        void AddMaterial(const SPtr<Material>& material);

        void AddChild(const SPtr<ModelNode>& child);
        void RemoveChild(const SPtr<ModelNode>& child);

        const std::string& GetName() const;
        const glm::mat4& GetTransform() const;
        SPtr<ModelNode> GetParent() const;

        std::size_t GetMeshesCount() const;
        std::size_t GetMaterialsCount() const;
        std::size_t GetChildrenCount() const;

        void SetName(const std::string& name);
        void SetTransform(const glm::mat4& transform);
        void SetParent(const SPtr<ModelNode>& parent);

        const std::vector<SPtr<Mesh>>& GetMeshes() const;
        const std::vector<SPtr<Material>>& GetMaterials() const;
        const std::vector<SPtr<ModelNode>>& GetChildren() const;

    private:

        std::string _name;
        glm::mat4 _transform;
        WPtr<ModelNode> _parent;
        
        std::vector<SPtr<Mesh>> _meshes;
        std::vector<SPtr<Material>> _materials;
        std::vector<SPtr<ModelNode>> _children;
    };

    class Model
    {
    public:
        Model() : root(MakeSPtr<ModelNode>("Root")) {}
        ~Model() = default;
    
        SPtr<ModelNode> root;

    };
}    
