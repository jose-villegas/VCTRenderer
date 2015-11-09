#include "stdafx.h"
#include "camera.h"
Camera * Camera::mainCamera = nullptr;

Camera::Camera() : clipPlaneFar(100.0f), clipPlaneNear(1.0f),
    horizontalFoV(60.0f), aspectRatio(16.0f / 9.0f)
{
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->lookAt = glm::vec3(0.0f, 0.0f, 1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    // new cameras always as default
    SetMainCamera();
}


Camera::~Camera()
{
}

glm::mat4x4 Camera::GetViewMatrix() const
{
    return glm::lookAt(
               this->position,
               this->lookAt,
               this->up
           );
}

glm::mat4x4 Camera::GetProjecctionMatrix() const
{
    return glm::perspective(
               glm::degrees(this->horizontalFoV),
               this->aspectRatio,
               this->clipPlaneNear,
               this->clipPlaneFar
           );
}

void Camera::SetMainCamera()
{
    mainCamera = this;
}

Camera * Camera::Main()
{
    // return mainCamera;
    return mainCamera;
}
