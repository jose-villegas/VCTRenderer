#include "bounding_volume.h"

#include <glm/detail/func_common.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/detail/func_geometric.hpp>

void BoundingVolume::MinPoint(const glm::vec3 &val)
{
    if (!original)
    {
        this->boundaries[0] = min(this->boundaries[0], val);
    }
    else
    {
        original->boundaries[0] = min(original->boundaries[0], val);
    }

    BoundariesChanged();
}

void BoundingVolume::MaxPoint(const glm::vec3 &val)
{
    if (!original)
    {
        this->boundaries[1] = max(this->boundaries[1], val);
    }
    else
    {
        original->boundaries[1] = max(original->boundaries[1], val);
    };

    BoundariesChanged();
}

void BoundingVolume::Reset()
{
    this->boundaries[1] = glm::vec3(std::numeric_limits<float>::lowest());
    this->boundaries[0] = glm::vec3(std::numeric_limits<float>::infinity());

    if (original)
    {
        original.reset();
    }
}

const glm::vec3 &BoundingVolume::MinPoint(bool transformed) const
{
    return transformed || !original ? boundaries[0] : original->boundaries[0];
}

const glm::vec3 &BoundingVolume::MaxPoint(bool transformed) const
{
    return transformed || !original ? boundaries[1] : original->boundaries[1];
}

void BoundingVolume::Transform(const glm::mat4x4 &model)
{
    if (!original) { original = std::make_shared<BoundingVolume>(*this); }

    glm::vec4 right = row(model, 0);
    glm::vec4 up = row(model, 1);
    glm::vec4 backward = row(model, 2);;
    glm::vec3 translation = glm::vec3(row(model, 3));
    glm::vec3 a[3], b[3];
    a[0] = glm::vec3(right * original->boundaries[0].x);
    a[1] = glm::vec3(up * original->boundaries[0].y);
    a[2] = glm::vec3(backward * original->boundaries[0].z);
    b[0] = glm::vec3(right * original->boundaries[1].x);
    b[1] = glm::vec3(up * original->boundaries[1].y);
    b[2] = glm::vec3(backward * original->boundaries[1].z);
    glm::vec3 min = glm::min(a[0], b[0]) + glm::min(a[1], b[1]) +
                    glm::min(a[2], b[2]) + translation;
    glm::vec3 max = glm::max(a[0], b[0]) + glm::max(a[1], b[1]) +
                    glm::max(a[2], b[2]) + translation;
    this->boundaries[0] = min;
    this->boundaries[1] = max;
    BoundariesChanged();
}

BoundingVolume::BoundingVolume() : original(nullptr)
{
    Reset();
}

BoundingVolume::BoundingVolume(const glm::vec3 &min,
                               const glm::vec3 &max) : original(nullptr)
{
    Reset();
    MinPoint(min);
    MaxPoint(max);
}

BoundingVolume::~BoundingVolume()
{
}

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::BoundariesChanged()
{
    radius = distance(MinPoint(true), MaxPoint(true)) * 0.5f;
    center = (MinPoint(true) + MaxPoint(true)) / 2.0f;
}

float BoundingSphere::Radius() const
{
    return radius;
}

const glm::vec3 &BoundingSphere::Center() const
{
    return center;
}

BoundingSphere::BoundingSphere(const BoundingVolume &volume) :
    BoundingVolume(volume)
{
    radius = distance(MinPoint(true), MaxPoint(true)) * 0.5f;
    center = (MinPoint(true) + MaxPoint(true)) / 2.0f;
}
