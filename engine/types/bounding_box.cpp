#include "bounding_box.h"

#include <glm/detail/func_common.hpp>
#include <glm/gtc/matrix_access.hpp>

void BoundingBox::MinPoint(const glm::vec3 &val)
{
    if (!original)
    {
        this->boundaries[0] = min(this->boundaries[0], val);
        this->center = (boundaries[1] + boundaries[0]) * 0.5f;
        this->extent = (boundaries[1] - boundaries[0]) * 0.5f;
    }
    else
    {
        original->boundaries[0] = min(original->boundaries[0], val);
        original->center = (original->boundaries[1] + original->boundaries[0]) * 0.5f;
        original->extent = (original->boundaries[1] - original->boundaries[0]) * 0.5f;
    }
}

void BoundingBox::MaxPoint(const glm::vec3 &val)
{
    if (!original)
    {
        this->boundaries[1] = max(this->boundaries[1], val);
        this->center = (boundaries[1] + boundaries[0]) * 0.5f;
        this->extent = (boundaries[1] - boundaries[0]) * 0.5f;
    }
    else
    {
        original->boundaries[1] = max(original->boundaries[1], val);
        original->center = (original->boundaries[1] + original->boundaries[0]) * 0.5f;
        original->extent = (original->boundaries[1] - original->boundaries[0]) * 0.5f;
    };
}

void BoundingBox::Reset()
{
    this->boundaries[1] = glm::vec3(std::numeric_limits<float>::lowest());
    this->boundaries[0] = glm::vec3(std::numeric_limits<float>::infinity());

    if (original)
    {
        original.reset();
    }
}

const glm::vec3 &BoundingBox::MinPoint(bool transformed) const
{
    return transformed || !original ? boundaries[0] : original->boundaries[0];
}

const glm::vec3 &BoundingBox::MaxPoint(bool transformed) const
{
    return transformed || !original ? boundaries[1] : original->boundaries[1];
}

const glm::vec3 &BoundingBox::Center(bool transformed) const
{
    return transformed || !original ? center : original->center;
}

const glm::vec3 &BoundingBox::Extent(bool transformed) const
{
    return transformed || !original ? extent : original->extent;
}

void BoundingBox::Transform(const glm::mat4x4 &model)
{
    if (!original) { original = std::make_shared<BoundingBox>(*this); }

    static glm::vec4 right, up, backward;
    static glm::vec3 translation;
    static std::array<glm::vec3, 6> aligned;
    right = row(model, 0); up = row(model, 1); backward = row(model, 2);;
    translation = glm::vec3(row(model, 3));
    // extract aligned min and max points
    aligned[0] = glm::vec3(right * original->boundaries[0].x);
    aligned[1] = glm::vec3(up * original->boundaries[0].y);
    aligned[2] = glm::vec3(backward * original->boundaries[0].z);
    aligned[3] = glm::vec3(right * original->boundaries[1].x);
    aligned[4] = glm::vec3(up * original->boundaries[1].y);
    aligned[5] = glm::vec3(backward * original->boundaries[1].z);
    // reset values with min and max of new vertices
    this->boundaries[0] = min(aligned[0], aligned[3]) +
                          min(aligned[1], aligned[4]) +
                          min(aligned[2], aligned[5]) + translation;
    this->boundaries[1] = max(aligned[0], aligned[3]) +
                          max(aligned[1], aligned[4]) +
                          max(aligned[2], aligned[5]) + translation;
    this->center = (boundaries[1] + boundaries[0]) * 0.5f;
    this->extent = (boundaries[1] - boundaries[0]) * 0.5f;
}

BoundingBox::BoundingBox() : original(nullptr)
{
    Reset();
}

BoundingBox::~BoundingBox()
{
}