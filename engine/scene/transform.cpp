#include "Transform.h"

#include <glm/gtx/transform.hpp>


Transform::Transform()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    forward = glm::vec3(0.0f, 0.0f, 1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
}


Transform::~Transform()
{
}

const glm::vec3 &Transform::Position() const
{
    return position;
}

void Transform::Position(const glm::vec3 &val)
{
    Position(val.x, val.y, val.z);
}

void Transform::Position(float x, float y, float z)
{
    position.x = x; position.y = y; position.z = z;
    ComputeMatrix();
}

const glm::quat &Transform::Rotation() const
{
    return rotation;
}

void Transform::Rotation(const glm::quat &val)
{
    Rotation(val.x, val.y, val.z, val.w);;
}

void Transform::Rotation(float w, float x, float y, float z)
{
    rotation = glm::quat(w, x, y, z);
    ComputeMatrix();
}

const glm::vec3 &Transform::Scale() const
{
    return scale;
}

void Transform::Scale(const glm::vec3 &val)
{
    Scale(val.x, val.y, val.z);
}

void Transform::Scale(float x, float y, float z)
{
    scale.x = x; scale.y = y; scale.z = z;
    ComputeMatrix();
}

const glm::mat4x4 &Transform::ToMatrix() const
{
    return transformation;
}

void Transform::ComputeMatrix()
{
    transformation = translate(position) *
                     mat4_cast(rotation) *
                     glm::scale(scale);
}
