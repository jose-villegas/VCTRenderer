#include "camera.h"
#include <glm/gtc/matrix_transform.inl>

Camera::~Camera()
{
}

Camera::Camera() : clipPlaneFar(1000.0f), clipPlaneNear(0.3f),
    horizontalFoV(60.0f), aspectRatio(16.0f / 9.0f)
{
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->lookAt = glm::vec3(0.0f, 0.0f, 1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    viewValuesChanged = projectionValuesChanged = frustumValuesChanged = true;
}

float Camera::ClipPlaneFar() const
{
    return clipPlaneFar;
}

void Camera::ClipPlaneFar(float val)
{
    clipPlaneFar = glm::max(val, 0.01f);
    projectionValuesChanged = true;
}

float Camera::ClipPlaneNear() const
{
    return clipPlaneNear;
}

void Camera::ClipPlaneNear(float val)
{
    clipPlaneNear = glm::max(val, 0.01f);
    projectionValuesChanged = true;
}

float Camera::HorizontalFoV() const
{
    return horizontalFoV;
}

void Camera::HorizontalFoV(float val)
{
    horizontalFoV = glm::clamp(val, 1.0f, 179.0f);
    projectionValuesChanged = true;
}

float Camera::AspectRatio() const
{
    return aspectRatio;
}

void Camera::AspectRatio(float val)
{
    aspectRatio = val;
    projectionValuesChanged = true;
}

const glm::vec3 &Camera::LookAt() const
{
    return lookAt;
}

void Camera::LookAt(const glm::vec3 &val)
{
    lookAt = val;
    viewValuesChanged = true;
}

const glm::vec3 &Camera::Position() const
{
    return position;
}

void Camera::Position(const glm::vec3 &val)
{
    position = val;
    viewValuesChanged = true;
}

const glm::vec3 &Camera::Up() const
{
    return up;
}

void Camera::Up(const glm::vec3 &val)
{
    up = val;
    viewValuesChanged = true;
}

const glm::mat4x4 &Camera::ViewMatrix()
{
    if (viewValuesChanged)
    {
        ComputeViewMatrix();
        frustumValuesChanged = true;
        viewValuesChanged = false;
    }

    return viewMatrix;
}

const glm::mat4x4 &Camera::ProjectionMatrix()
{
    if (projectionValuesChanged)
    {
        ComputeProjectionMatrix();
        frustumValuesChanged = true;
        projectionValuesChanged = false;
    }

    return projectionMatrix;
}

bool Camera::InFrustum(const BoundingVolume &volume)
{
    if (frustumValuesChanged)
    {
        frustum.CalculatePlanes(projectionMatrix * viewMatrix);
        frustumValuesChanged = false;
    }

    return frustum.VolumeInFrustum(volume);
}

void Camera::ComputeViewMatrix()
{
    viewMatrix = glm::lookAt(position, lookAt, up);
}

void Camera::ComputeProjectionMatrix()
{
    projectionMatrix = glm::perspective
                       (
                           glm::degrees(horizontalFoV),
                           aspectRatio,
                           clipPlaneNear, clipPlaneFar
                       );
}