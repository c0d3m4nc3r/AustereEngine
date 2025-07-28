#include "Game/Nodes.hpp"

#include <AE/Core/Engine.hpp>
#include <AE/Core/Window.hpp>
#include <AE/Input/Manager.hpp>
#include <AE/Rendering/Camera.hpp>

const AE::Key MOVE_FORWARD_KEY = AE::Key::W;
const AE::Key MOVE_BACKWARD_KEY = AE::Key::S;
const AE::Key MOVE_LEFT_KEY = AE::Key::A;
const AE::Key MOVE_RIGHT_KEY = AE::Key::D;
const AE::Key MOVE_UP_KEY = AE::Key::Space;
const AE::Key MOVE_DOWN_KEY = AE::Key::LShift;

bool CameraNode::OnInitialize()
{
    camera = std::make_shared<AE::Camera>();
    camera->transform.SetParent(&transform);

    SDL_SetWindowRelativeMouseMode(engine->GetWindow()->GetWindow(), true);

    return true;
}

void CameraNode::OnDestroy()
{
    camera.reset();
}

void CameraNode::OnUpdate()
{
    AE::InputManager* inputMgr = engine->GetInputManager();
    AE::Keyboard* keyboard = inputMgr->GetKeyboard();
    AE::Mouse* mouse = inputMgr->GetMouse();
    AE::Window* window = engine->GetWindow();

    camera->SetAspectRatio(window->GetAspectRatio());

    velocity = glm::mix(velocity, glm::vec3(0.0f), 0.3);

    if (keyboard->IsKeyDown(MOVE_FORWARD_KEY)) {
        velocity += transform.GetForward() * moveSpeed;
    } else if (keyboard->IsKeyDown(MOVE_BACKWARD_KEY)) {
        velocity -= transform.GetForward() * moveSpeed;
    }

    if (keyboard->IsKeyDown(MOVE_RIGHT_KEY)) {
        velocity += transform.GetRight() * moveSpeed;
    } else if (keyboard->IsKeyDown(MOVE_LEFT_KEY)) {
        velocity -= transform.GetRight() * moveSpeed;
    }

    if (keyboard->IsKeyDown(MOVE_UP_KEY)) {
        velocity += glm::vec3(0.0f, 1.0f, 0.0f) * moveSpeed;
    } else if (keyboard->IsKeyDown(MOVE_DOWN_KEY)) {
        velocity -= glm::vec3(0.0f, 1.0f, 0.0f) * moveSpeed;
    }

    const float maxSpeed = 50.0f;
    if (glm::length(velocity) > maxSpeed)
        velocity = glm::normalize(velocity) * maxSpeed;

    if (keyboard->IsKeyPressed(AE::Key::R))
    {
        transform.SetPosition(glm::vec3(0.0f));
        transform.SetRotation(glm::identity<glm::quat>());
        velocity = glm::vec3(0.0f);
        pitch = 0.0f; yaw = 0.0f;
    }

    transform.Translate(velocity * engine->GetDeltaTime());

    int winWidth, winHeight;
    window->GetSize(&winWidth, &winHeight);

    glm::vec2 mouseDelta = mouse->GetDelta();

    yaw -= mouseDelta.x * mouseSensitivity;
    pitch -= mouseDelta.y * mouseSensitivity;

    const float pitchLimit = glm::radians(89.0f);
    pitch = glm::clamp(pitch, -pitchLimit, pitchLimit);

    glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
    glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
    glm::quat orientation = qYaw * qPitch;

    transform.SetRotation(orientation);
}