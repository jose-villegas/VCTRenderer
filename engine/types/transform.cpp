#include "Transform.h"

#include "../util/const_definitions.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/orthonormalize.inl>
#include <glm/gtx/euler_angles.hpp>

Transform::Transform()
{
    position = Vector3::zero;
    scale = Vector3::one;
    rotation = glm::quat(Vector3::zero);
    forward = Vector3::forward;
    up = Vector3::up;
    right = Vector3::right;
    transformation = translate(position) *
                     mat4_cast(rotation) *
                     glm::scale(scale);
    UpdateCoordinates();
}

Transform::~Transform()
{
}

void Transform::Position(const glm::vec3 &val)
{
    if(position != val)
    {
        position = val;
        UpdateTransformMatrix();
    }
}

void Transform::Rotation(const glm::quat &val)
{
    if(rotation != val)
    {
        this->angles = eulerAngles(val);
        rotation = val;
        UpdateCoordinates();
        UpdateTransformMatrix();
    }
}

void Transform::Rotation(const glm::vec3 &angles)
{
    if(this->angles != angles)
    {
        this->angles = angles;
        auto rotationX = angleAxis(angles.x, Vector3::right);
        auto rotationY = angleAxis(angles.y, Vector3::up);
        auto rotationZ = angleAxis(angles.z, Vector3::forward);
        // final composite rotation
        rotation = normalize(rotationZ * rotationX * rotationY);
        // rotate direction vectors
        UpdateCoordinates();
        UpdateTransformMatrix();
    }
}

void Transform::Scale(const glm::vec3 &val)
{
    if(scale != val)
    {
        scale = val;
        UpdateTransformMatrix();
    }
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

void Transform::UpdateTransformMatrix()
{
    transformation = translate(position) *
                     mat4_cast(rotation) *
                     glm::scale(scale);
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
    if (forward != val)
    {
        forward = val;
        UpdateTransformMatrix();
    }
}

void Transform::Right(const glm::vec3 &val)
{
    if (right != val)
    {
        right = val;
        UpdateTransformMatrix();
    }
}

void Transform::Up(const glm::vec3 &val)
{
    if (up != val)
    {
        up = val;
        UpdateTransformMatrix();
    }
}

const glm::vec3 &Transform::Angles() const
{
    return angles;
}

const glm::mat4x4 &Transform::Matrix() const
{
    return transformation;
}