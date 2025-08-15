#include "Resources/Model.hpp"

#include <algorithm>

namespace AE
{
    ModelNode::ModelNode(const std::string& name, const glm::mat4& transform)
        : _name(name), _transform(transform) {}
   
    void ModelNode::AddMesh(const SPtr<Mesh>& mesh) { _meshes.push_back(mesh); }
    void ModelNode::AddMaterial(const SPtr<Material>& material) { _materials.push_back(material); }
    
    void ModelNode::AddChild(const SPtr<ModelNode>& child)
    {
        if (child)
        {
            child->SetParent(shared_from_this());
            _children.push_back(child);
        }
    }
    
    void ModelNode::RemoveChild(const SPtr<ModelNode>& child)
    {
        _children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
    }

    const std::string& ModelNode::GetName() const { return _name; }
    const glm::mat4& ModelNode::GetTransform() const { return _transform; }
    SPtr<ModelNode> ModelNode::GetParent() const { return _parent.lock(); }

    std::size_t ModelNode::GetMeshesCount() const { return _meshes.size(); }
    std::size_t ModelNode::GetMaterialsCount() const { return _materials.size(); }
    std::size_t ModelNode::GetChildrenCount() const { return _children.size(); }
    
    const std::vector<SPtr<Mesh>>& ModelNode::GetMeshes() const { return _meshes; }
    const std::vector<SPtr<Material>>& ModelNode::GetMaterials() const { return _materials; }
    const std::vector<SPtr<ModelNode>>& ModelNode::GetChildren() const { return _children; }
    
    void ModelNode::SetName(const std::string& name) { _name = name; }
    void ModelNode::SetTransform(const glm::mat4& transform) { _transform = transform; }
    void ModelNode::SetParent(const SPtr<ModelNode>& parent) { _parent = parent; }    
}
