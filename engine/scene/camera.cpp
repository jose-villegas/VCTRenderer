#include "camera.h"

#include "../types/bounding_box.h"

#include <glm/detail/func_common.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::~Camera()
{
}

Camera::Camera() : clipPlaneFar(10000.0f), clipPlaneNear(0.3f),
    horizontalFoV(60.0f), aspectRatio(16.0f / 9.0f)
{
    name = "Default Camera";
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
    projectionValuesChanged |= val != clipPlaneFar;
    clipPlaneFar = glm::max(val, 0.01f);
}

float Camera::ClipPlaneNear() const
{
    return clipPlaneNear;
}

void Camera::ClipPlaneNear(float val)
{
    projectionValuesChanged |= val != clipPlaneNear;
    clipPlaneNear = glm::max(val, 0.01f);
}

float Camera::HorizontalFoV() const
{
    return horizontalFoV;
}

void Camera::HorizontalFoV(float val)
{
    projectionValuesChanged |= val != horizontalFoV;
    horizontalFoV = glm::clamp(val, 1.0f, 179.0f);
}

float Camera::AspectRatio() const
{
    return aspectRatio;
}

void Camera::AspectRatio(float val)
{
    projectionValuesChanged |= val != aspectRatio;
    aspectRatio = val;
}

const glm::vec3 &Camera::LookAt() const
{
    return lookAt;
}

void Camera::LookAt(const glm::vec3 &val)
{
    viewValuesChanged |= val != lookAt;
    lookAt = val;
}

const glm::vec3 &Camera::Position() const
{
    return position;
}

void Camera::Position(const glm::vec3 &val)
{
    viewValuesChanged |= val != position;
    position = val;
}

const glm::vec3 &Camera::Up() const
{
    return up;
}

void Camera::Up(const glm::vec3 &val)
{
    viewValuesChanged |= val != up;
    up = val;
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

bool Camera::ParametersChanged() const
{
    return projectionValuesChanged || viewValuesChanged;
}

bool Camera::InFrustum(const BoundingBox &volume)
{
    if (frustumValuesChanged)
    {
        frustum.ExtractPlanes(projectionMatrix * viewMatrix);
        frustumValuesChanged = false;
    }

    return frustum.InFrustum(volume);
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