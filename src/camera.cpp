#include "camera.h"
#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

#include <iostream>
#include <ostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
    : width(800)
    , height(600)
    , cameraPosition(glm::vec3(0.0f, 0.0f, 0.0f))
{
}

Camera::Camera(int width, int height, glm::vec3 cameraPosition)
    : width(width)
    , height(height)
    , cameraPosition(cameraPosition)
{
}

void print_v3(glm::vec3& vec)
{
    std::cout << "[" << vec.x << ", ";
    std::cout << vec.y << ", ";
    std::cout << vec.z << "]\n";
}

auto Camera::updateCameraSpeed(float speed) -> void
{
    this->cameraSpeed = speed / 1000;
}

auto Camera::processKeyboard(CameraMovement direction) -> void
{
    if (direction == FORWARD) {
        cameraPosition += cameraSpeed * cameraDirection;
    }
    if (direction == BACKWARD) {
        cameraPosition += cameraSpeed * -cameraDirection;
    }
    if (direction == LEFT) {
        cameraPosition += cameraSpeed * -glm::normalize(glm::cross(cameraDirection, cameraUp));
    }
    if (direction == RIGHT) {
        cameraPosition += cameraSpeed * glm::normalize(glm::cross(cameraDirection, cameraUp));
    }
}

auto Camera::getViewMatrix() -> glm::mat4
{
    auto la = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);

    return la;
}

auto Camera::getProjectionMatrix(float FOV) -> glm::mat4
{
    return glm::perspective(FOV, width / (float)height, 0.1f, 100.0f);
}

namespace glm {
template <length_t T, typename F, qualifier Q>
class vec {
    std::ostream& operator<<(std::ostream& os)
    {
        os << "[";
        os << this->x << ", ";
        os << this->y << ", ";
        os << this->z;
        return os << "]\n";
    }
};
}
