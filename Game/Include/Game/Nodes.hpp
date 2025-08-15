#pragma once

#include <AE/Scene/Node.hpp>
#include <AE/World/Skybox.hpp>

namespace AE
{
    class Camera;
    class Model;
}

class CameraNode : public AE::SceneNode
{
public:
    CameraNode(const std::string& name = "CameraNode")
        : AE::SceneNode(name) {}

    AE::SPtr<AE::Camera> camera;

    float moveSpeed = 2.0f;
    float mouseSensitivity = 0.001f;
    
    float yaw = 0.0f;
    float pitch = 0.0f;

    glm::vec3 velocity{0.0f};
    
    bool OnInitialize() override;
    void OnDestroy() override;
    void OnUpdate() override;
};

class TestNode : public AE::SceneNode
{
public:
    
    TestNode(const std::string& name = "TestNode")
        : AE::SceneNode(name) {}
    
    bool OnInitialize() override;
    void OnDestroy() override;
    void OnUpdate() override;
    void OnRender() override;

private:
    AE::SPtr<AE::Model> _testModel;
};

class TestSceneRoot : public AE::SceneNode
{
public:

    TestSceneRoot(const std::string& name = "Root")
        : AE::SceneNode(name) {}
    
    bool OnInitialize() override;
    void OnDestroy() override;
    void OnRender() override;
    
private:

    AE::SPtr<CameraNode> _cameraNode;
    AE::UPtr<AE::Skybox> _skybox;
};
