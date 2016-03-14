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
}

float Camera::ClipPlaneFar() const
{
    return clipPlaneFar;
}

void Camera::ClipPlaneFar(float val)
{
    clipPlaneFar = glm::max(val, 0.01f);
}

float Camera::ClipPlaneNear() const
{
    return clipPlaneNear;
}

void Camera::ClipPlaneNear(float val)
{
    clipPlaneNear = glm::max(val, 0.01f);
}

float Camera::HorizontalFoV() const
{
    return horizontalFoV;
}

void Camera::HorizontalFoV(float val)
{
    horizontalFoV = glm::clamp(val, 1.0f, 179.0f);
}

float Camera::AspectRatio() const
{
    return aspectRatio;
}

void Camera::AspectRatio(float val)
{
    aspectRatio = val;
}

glm::vec3 Camera::LookAt() const
{
    return transform.Position() + transform.Forward();
}

const glm::mat4x4 &Camera::ViewMatrix()
{
    return viewMatrix = lookAt
                        (
                            transform.Position(),
                            LookAt(),
                            transform.Up()
                        );;
}

const glm::mat4x4 &Camera::ProjectionMatrix()
{
    return projectionMatrix = glm::perspective
                              (
                                  glm::radians(horizontalFoV),
                                  aspectRatio,
                                  clipPlaneNear, clipPlaneFar
                              );;
}

const glm::mat4x4 &Camera::InverseViewMatrix()
{
    return inverseViewMatrix = inverse(ViewMatrix());
}

const glm::mat4x4 &Camera::InverseProjectionMatrix()
{
    return inverseProjectionMatrix = inverse(ProjectionMatrix());
}

bool Camera::InFrustum(const BoundingBox &volume)
{
    frustum.ExtractPlanes(ProjectionMatrix() * ViewMatrix());
    return frustum.InFrustum(volume);
}