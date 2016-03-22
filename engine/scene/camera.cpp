#include "camera.h"

#include "../types/bounding_box.h"

#include <glm/detail/func_common.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::~Camera()
{
}

void Camera::Projection(enum class ProjectionMode mode)
{
    this->mode = mode;
}

Camera::Camera() : clipPlaneFar(10000.0f), clipPlaneNear(0.3f),
    fieldOfView(glm::radians(60.0f)), aspectRatio(16.0f / 9.0f),
    mode(ProjectionMode::Perspective)
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

const float &Camera::FieldOfView() const
{
    return fieldOfView;
}

void Camera::FieldOfView(const float &val)
{
    fieldOfView = glm::clamp(val, 0.0f, glm::pi<float>());
}

float Camera::AspectRatio() const
{
    return aspectRatio;
}

void Camera::AspectRatio(float val)
{
    aspectRatio = val;
}

void Camera::OrthoRect(const glm::vec4 &rect)
{
    orthoRect = rect;
}

const glm::vec4 &Camera::OrthoRect() const
{
    return orthoRect;
}

glm::vec3 Camera::LookAt() const
{
    return transform.Position() + transform.Forward();
}

const glm::mat4x4 &Camera::ViewMatrix()
{
    return viewMatrix = lookAt(transform.Position(), LookAt(), transform.Up());
}

const glm::mat4x4 &Camera::ProjectionMatrix()
{
    if (mode == ProjectionMode::Perspective) {
        return projectionMatrix = glm::perspective(fieldOfView, aspectRatio,
                                  clipPlaneNear, clipPlaneFar);
    }

    return projectionMatrix = glm::ortho(orthoRect.x, orthoRect.y, orthoRect.z,
                                         orthoRect.w, clipPlaneNear, clipPlaneFar);
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