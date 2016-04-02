#include "camera.h"

#include "../types/bounding_box.h"

#include <glm/detail/func_common.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::~Camera()
{
}

void Camera::Projection(enum class ProjectionMode mode)
{
    if(this->mode != mode)
    {
        this->mode = mode;
        UpdateProjectionMatrix();
    }
}

Camera::Camera() : clipPlaneFar(10000.0f), clipPlaneNear(0.3f),
    fieldOfView(glm::radians(60.0f)), aspectRatio(16.0f / 9.0f),
    mode(ProjectionMode::Perspective)
{
    name = "Default Camera";
    UpdateProjectionMatrix();
}

float Camera::ClipPlaneFar() const
{
    return clipPlaneFar;
}

void Camera::ClipPlaneFar(float val)
{
    auto res = glm::max(val, 0.01f);

    if(res != clipPlaneFar)
    {
        clipPlaneFar = res;
        UpdateProjectionMatrix();
    }
}

float Camera::ClipPlaneNear() const
{
    return clipPlaneNear;
}

void Camera::ClipPlaneNear(float val)
{
    auto res = glm::max(val, 0.01f);

    if (res != clipPlaneNear)
    {
        clipPlaneNear = res;
        UpdateProjectionMatrix();
    }
}

const float &Camera::FieldOfView() const
{
    return fieldOfView;
}

void Camera::FieldOfView(const float &val)
{
    auto res = glm::clamp(val, 0.0f, glm::pi<float>());

    if (res != fieldOfView)
    {
        fieldOfView = res;
        UpdateProjectionMatrix();
    }
}

float Camera::AspectRatio() const
{
    return aspectRatio;
}

void Camera::AspectRatio(float val)
{
    if (val != aspectRatio)
    {
        aspectRatio = val;
        UpdateProjectionMatrix();
    }
}

void Camera::OrthoRect(const glm::vec4 &rect)
{
    if (rect != orthoRect)
    {
        orthoRect = rect;
        UpdateProjectionMatrix();
    }
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

const glm::mat4x4 &Camera::ProjectionMatrix() const
{
    return projectionMatrix;
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

void Camera::UpdateProjectionMatrix()
{
    if (mode == ProjectionMode::Perspective)
    {
        projectionMatrix = glm::perspective(fieldOfView, aspectRatio,
                                            clipPlaneNear, clipPlaneFar);
    }
    else
    {
        projectionMatrix = glm::ortho(orthoRect.x, orthoRect.y, orthoRect.z,
                                      orthoRect.w, clipPlaneNear, clipPlaneFar);
    }
}
