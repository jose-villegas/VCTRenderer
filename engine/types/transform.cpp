#include "Transform.h"

#include "../util/const_definitions.h"

#include <glm/gtx/transform.hpp>

Transform::Transform() : changed(false)
{
    position = Vector3::zero;
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    scale = Vector3::zero;
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

const glm::vec3 &Transform::Scale() const
{
    return scale;
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
