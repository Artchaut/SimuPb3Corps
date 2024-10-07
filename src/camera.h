#include "glm/fwd.hpp"
#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

#ifndef CAMERA_H

#define CAMERA_H

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

class Camera {
private:
    glm::vec3 cameraPosition;
    glm::vec3 cameraDirection = glm::vec3(0.0f, -4.0f, -2.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);

    int width, height;

    float cameraSpeed = 0.001f;


public:
    Camera();
    explicit Camera(int width, int height, glm::vec3 cameraPosition = glm::vec3(0.0f, 4.0f, 2.0f));

    auto updateCameraSpeed(float speed) -> void;

    auto getViewMatrix() -> glm::mat4;

    auto getProjectionMatrix(float FOV) -> glm::mat4;

    auto processKeyboard(CameraMovement direction) -> void;
};

#endif // !CAMERA_H
