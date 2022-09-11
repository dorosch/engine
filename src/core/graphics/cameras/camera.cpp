#include "core/graphics/cameras/camera.hpp"

using namespace Engine::Graphics::Camera;


Camera::Camera() {
    yaw = -90.0f;
    pitch = 0.0f;

    zoom = 60.f;
    speed = 10.0f;
    sensitivity = 0.25f;

    position = glm::vec3(0.0f, 0.0f, 0.0f);
    direction = glm::vec3(0.0f, 0.0f, 0.0f);

    projection = Projection::PERSPECTIVE;

    updateVectors();
}


void Camera::updateVectors() {
    glm::vec3 unitY = glm::vec3(0.0f, 1.0f, 0.0f);

    // Calculate the new front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Also re-calculate the right and up vector
    right = glm::normalize(glm::cross(front, unitY));
    up = glm::normalize(glm::cross(right, front));
}


glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}


glm::mat4 Camera::getProjectionMatrix() {
    // TODO: Pass viewport information to the camera
    float screenWidth = 1920;
    float screenHeight = 1080;

    switch (projection) {
    case Projection::PERSPECTIVE:
        return glm::perspective(glm::radians(zoom), screenWidth / screenHeight, 0.1f, 1000.0f);
    case Projection::ORTHOGRAPHIC:
        return glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, 0.5f, 10.0f);
    default:
        throw UnsupportedCameraProjection();
    }
}
