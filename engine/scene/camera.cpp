#include "camera.h"

#include "../types/bounding_box.h"

#include <glm/detail/func_common.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::~Camera()
{
}

void Camera::DoFrustumCulling(const bool val)
{
    doFrustumCulling = val;
}

void Camera::Projection(const ProjectionMode &mode)
{
    if (this->mode != mode)
    {
        this->mode = mode;
        UpdateProjectionMatrix();
    }
}

void Camera::UpdateTransformMatrix()
{
    viewMatrix = lookAt(transform.Position(), LookAt(), transform.Up());
    // view dependant
    inverseViewMatrix = inverse(viewMatrix);
    viewProjectionMatrix = projectionMatrix * viewMatrix;
    frustum.ExtractPlanes(viewProjectionMatrix);
}

Camera::Camera() : clipPlaneFar(10000.0f), clipPlaneNear(0.3f),
    fieldOfView(glm::radians(60.0f)), aspectRatio(16.0f / 9.0f),
    mode(ProjectionMode::Perspective)
{
    name = "Default Camera";
    viewMatrix = lookAt(transform.Position(), LookAt(), transform.Up());
    inverseViewMatrix = inverse(viewMatrix);
    doFrustumCulling = true;
    UpdateProjectionMatrix();
}

const float &Camera::ClipPlaneFar() const
{
    return clipPlaneFar;
}

void Camera::ClipPlaneFar(const float &val)
{
    auto res = glm::max(val, 0.01f);

    if(res != clipPlaneFar)
    {
        clipPlaneFar = res;
        UpdateProjectionMatrix();
    }
}

const float &Camera::ClipPlaneNear() const
{
    return clipPlaneNear;
}

void Camera::ClipPlaneNear(const float &val)
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

const float &Camera::AspectRatio() const
{
    return aspectRatio;
}

void Camera::AspectRatio(const float &val)
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

const glm::mat4x4 &Camera::ViewMatrix() const
{
    return viewMatrix;
}

const glm::mat4x4 &Camera::ProjectionMatrix() const
{
    return projectionMatrix;
}

const glm::mat4x4 &Camera::ViewProjectionMatrix() const
{
    return viewProjectionMatrix;
}

const glm::mat4x4 &Camera::InverseViewMatrix() const
{
    return inverseViewMatrix;
}

const glm::mat4x4 &Camera::InverseProjectionMatrix() const
{
    return inverseProjectionMatrix;
}

bool Camera::InFrustum(const BoundingBox &volume) const
{
    if (!doFrustumCulling) return true;

    return frustum.InFrustum(volume);
}

const CullingFrustum &Camera::Frustum() const
{
    return frustum;
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

    // projection dependant
    inverseProjectionMatrix = inverse(projectionMatrix);
    viewProjectionMatrix = projectionMatrix * viewMatrix;
    frustum.ExtractPlanes(viewProjectionMatrix);
}
