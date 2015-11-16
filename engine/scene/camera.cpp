#include "camera.h"
#include <glm/gtc/matrix_transform.inl>

Camera::Camera() : clipPlaneFar(100.0f), clipPlaneNear(1.0f),
    horizontalFoV(60.0f), aspectRatio(16.0f / 9.0f)
{
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->lookAt = glm::vec3(0.0f, 0.0f, 1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    // new cameras always as default
}

glm::mat4x4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, lookAt, up);
}

glm::mat4x4 Camera::GetProjecctionMatrix() const
{
    return glm::perspective(glm::degrees(horizontalFoV), aspectRatio,
                            clipPlaneNear, clipPlaneFar);
}