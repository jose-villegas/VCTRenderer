#include "Transform.h"

#include "../util/const_definitions.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/orthonormalize.inl>

Transform::Transform() : changed(false)
{
    forward = Vector3::forward;
    up = Vector3::up;
    right = Vector3::right;
    transformation = Matrix::identity4;
}

Transform::~Transform()
{
}

void Transform::Position(const glm::vec3 &val)
{
    position = val; changed = true;
}

void Transform::Rotation(const glm::quat &val)
{
    rotation = val; changed = true;
    this->angles = eulerAngles(val);
    UpdateCoordinates();
}

void Transform::Rotation(const glm::vec3 &angles)
{
    rotation = glm::quat(angles); changed = true;
    this->angles = angles;
    UpdateCoordinates();
}

void Transform::Scale(const glm::vec3 &val)
{
    scale = val; changed = true;
}

const glm::vec3 &Transform::Position() const
{
    return position;
}

const glm::quat &Transform::Rotation() const
{
    return rotation;
}

void Transform::UpdateCoordinates()
{
    forward = Vector3::forward * rotation;
    right = Vector3::right * rotation;
    up = Vector3::up * rotation;
}

const glm::vec3 &Transform::Scale() const
{
    return scale;
}

const glm::vec3 &Transform::Forward() const
{
    return forward;
}

const glm::vec3 &Transform::Right() const
{
    return right;
}

const glm::vec3 &Transform::Up() const
{
    return up;
}

const glm::vec3 &Transform::Angles()
{
    return angles;
}

const glm::mat4x4 &Transform::ToMatrix()
{
    if (changed)
    {
        transformation = translate(position) *
                         mat4_cast(rotation) *
                         glm::scale(scale);
    }

    return transformation;
}

void Transform::LookAt(const glm::vec3 &pos, const glm::vec3 &up)
{
    forward = normalize(pos);
    forward = orthonormalize(forward, up);
    right = cross(up, forward);
    rotation.w = sqrt(1.0f + right.x + up.y + forward.z) * 0.5f;
    auto recip = 1.0f / (4.0f * rotation.w);
    rotation.x = (up.z - forward.y) * recip;
    rotation.y = (forward.x - right.z) * recip;
    rotation.z = (right.y - up.x) * recip;
}
