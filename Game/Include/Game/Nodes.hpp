#pragma once

#include <AE/Scene/Node.hpp>

namespace AE
{
    class Shader;
    class Model;
    class Camera;
}

class CameraNode : public AE::SceneNode
{
public:
    CameraNode(const std::string& name = "CameraNode")
        : AE::SceneNode(name) {}

    std::shared_ptr<AE::Camera> camera;

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

    std::shared_ptr<AE::Shader> mainShader;
    std::shared_ptr<AE::Model> testModel;

    bool OnInitialize() override;
    void OnDestroy() override;
    void OnUpdate() override;
    void OnRender() override;
};
