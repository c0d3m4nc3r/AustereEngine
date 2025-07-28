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

        void AddMesh(const std::shared_ptr<Mesh>& mesh);
        void AddMaterial(const std::shared_ptr<Material>& material);

        void AddChild(const std::shared_ptr<ModelNode>& child);
        void RemoveChild(const std::shared_ptr<ModelNode>& child);

        const std::string& GetName() const;
        const glm::mat4& GetTransform() const;
        std::shared_ptr<ModelNode> GetParent() const;

        std::size_t GetMeshesCount() const;
        std::size_t GetMaterialsCount() const;
        std::size_t GetChildrenCount() const;

        void SetName(const std::string& name);
        void SetTransform(const glm::mat4& transform);
        void SetParent(const std::shared_ptr<ModelNode>& parent);

        const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const;
        const std::vector<std::shared_ptr<Material>>& GetMaterials() const;
        const std::vector<std::shared_ptr<ModelNode>>& GetChildren() const;

    private:

        std::string _name;
        glm::mat4 _transform;
        std::weak_ptr<ModelNode> _parent;
        
        std::vector<std::shared_ptr<Mesh>> _meshes;
        std::vector<std::shared_ptr<Material>> _materials;
        std::vector<std::shared_ptr<ModelNode>> _children;
    };

    class Model
    {
    public:
        Model() : root(std::make_shared<ModelNode>("Root")) {}
        ~Model() = default;
    
        std::shared_ptr<ModelNode> root;

    };
}    
