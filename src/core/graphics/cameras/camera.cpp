#include "core/graphics/cameras/camera.hpp"

using namespace Engine::Graphics::Camera;


Camera::Camera() {
    name = "Camera";
    camera = std::make_unique<Ecs::Component::Camera>();

    camera->yaw = -90.0f;
    camera->pitch = 0.0f;

    camera->zoom = 60.f;
    camera->speed = 10.0f;
    camera->sensitivity = 0.25f;

    transform->position = glm::vec3(0.0f, 0.0f, 0.0f);

    camera->projection = Projection::PERSPECTIVE;

    updateVectors();
}


void Camera::updateVectors() {
    glm::vec3 unitY = glm::vec3(0.0f, 1.0f, 0.0f);

    // Calculate the new front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    newFront.y = sin(glm::radians(camera->pitch));
    newFront.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->front = glm::normalize(newFront);

    // Also re-calculate the right and up vector
    camera->right = glm::normalize(glm::cross(camera->front, unitY));
    camera->up = glm::normalize(glm::cross(camera->right, camera->front));
}


glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(transform->position, transform->position + camera->front, camera->up);
}


glm::mat4 Camera::getProjectionMatrix() {
    // TODO: Pass viewport information to the camera
    float screenWidth = 1920;
    float screenHeight = 1080;

    switch (camera->projection) {
        case Projection::PERSPECTIVE:
            // TODO: Pass distance with viewport
            return glm::perspective(glm::radians(camera->zoom), screenWidth / screenHeight, 0.1f, 1000.0f);
        case Projection::ORTHOGRAPHIC:
            return glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, 0.5f, 10.0f);
        default:
            throw UnsupportedCameraProjection();
    }
}
