#include "Transform.h"

#include "../util/const_definitions.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/orthonormalize.inl>
#include <glm/gtx/euler_angles.hpp>

Transform::Transform() : changed(true)
{
    position = Vector3::zero;
    scale = Vector3::one;
    rotation = glm::quat(Vector3::zero);
    forward = Vector3::forward;
    up = Vector3::up;
    right = Vector3::right;
    transformation = Matrix();
    changed = false;
}

Transform::~Transform()
{
}

void Transform::Position(const glm::vec3 &val)
{
    changed |= position != val;

    if (!changed) { return; }

    position = val;
}

void Transform::Rotation(const glm::quat &val)
{
    changed |= rotation != val;

    if (!changed) { return; }

    this->angles = eulerAngles(val);
    rotation = val;
    UpdateCoordinates();
}

void Transform::Rotation(const glm::vec3 &angles)
{
    changed |= this->angles != angles;

    if (!changed) { return; }

    this->angles = angles;
    auto rotationX = angleAxis(angles.x, Vector3::right);
    auto rotationY = angleAxis(angles.y, Vector3::up);
    auto rotationZ = angleAxis(angles.z, Vector3::forward);
    // final composite rotation
    rotation = normalize(rotationZ * rotationX * rotationY);
    // rotate direction vectors
    UpdateCoordinates();
}

void Transform::Scale(const glm::vec3 &val)
{
    changed |= scale != val;

    if (!changed) { return; }

    scale = val;
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
    up = normalize(Vector3::up * rotation);
    right = normalize(Vector3::right * rotation);
    forward = normalize(Vector3::forward * rotation);
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

void Transform::Forward(const glm::vec3 &val)
{
    auto w = cross(Vector3::forward, val);
    auto q = glm::quat(1.f + dot(Vector3::forward, val), w.x, w.y, w.z);
    Rotation(normalize(q));
}

void Transform::Right(const glm::vec3 &val)
{
    auto w = cross(Vector3::right, val);
    auto q = glm::quat(1.f + dot(Vector3::right, val), w.x, w.y, w.z);
    Rotation(normalize(q));
}

void Transform::Up(const glm::vec3 &val)
{
    auto w = cross(Vector3::up, val);
    auto q = glm::quat(1.f + dot(Vector3::up, val), w.x, w.y, w.z);
    Rotation(normalize(q));
}

const glm::vec3 &Transform::Angles() const
{
    return angles;
}

const glm::mat4x4 &Transform::Matrix()
{
    if (changed)
    {
        transformation = translate(position) *
                         mat4_cast(rotation) *
                         glm::scale(scale);
    }

    return transformation;
}

const glm::mat4x4 &Transform::InverseMatrix()
{
    if (changed)
    {
        inverseTransformation = inverse(transformation);
    }

    return inverseTransformation;
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
    changed = true;
}
