#include "Resources/Model.hpp"

#include <algorithm>

namespace AE
{
    ModelNode::ModelNode(const std::string& name, const glm::mat4& transform)
        : _name(name), _transform(transform) {}
   
    void ModelNode::AddMesh(const std::shared_ptr<Mesh>& mesh) { _meshes.push_back(mesh); }
    void ModelNode::AddMaterial(const std::shared_ptr<Material>& material) { _materials.push_back(material); }
    
    void ModelNode::AddChild(const std::shared_ptr<ModelNode>& child)
    {
        if (child)
        {
            child->SetParent(shared_from_this());
            _children.push_back(child);
        }
    }
    
    void ModelNode::RemoveChild(const std::shared_ptr<ModelNode>& child)
    {
        _children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
    }

    const std::string& ModelNode::GetName() const { return _name; }
    const glm::mat4& ModelNode::GetTransform() const { return _transform; }
    std::shared_ptr<ModelNode> ModelNode::GetParent() const { return _parent.lock(); }

    std::size_t ModelNode::GetMeshesCount() const { return _meshes.size(); }
    std::size_t ModelNode::GetMaterialsCount() const { return _materials.size(); }
    std::size_t ModelNode::GetChildrenCount() const { return _children.size(); }
    
    const std::vector<std::shared_ptr<Mesh>>& ModelNode::GetMeshes() const { return _meshes; }
    const std::vector<std::shared_ptr<Material>>& ModelNode::GetMaterials() const { return _materials; }
    const std::vector<std::shared_ptr<ModelNode>>& ModelNode::GetChildren() const { return _children; }
    
    void ModelNode::SetName(const std::string& name) { _name = name; }
    void ModelNode::SetTransform(const glm::mat4& transform) { _transform = transform; }
    void ModelNode::SetParent(const std::shared_ptr<ModelNode>& parent) { _parent = parent; }    
}
