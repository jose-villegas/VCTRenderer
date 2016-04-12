#include "bounding_box.h"

#include <glm/detail/func_common.hpp>
#include <glm/gtc/matrix_access.hpp>

void BoundingBox::MinPoint(const glm::vec3 &val)
{
    this->boundaries[0] = min(this->boundaries[0], val);
    this->center = (boundaries[1] + boundaries[0]) * 0.5f;
    this->extent = (boundaries[1] - boundaries[0]) * 0.5f;
}

void BoundingBox::MaxPoint(const glm::vec3 &val)
{
    this->boundaries[1] = max(this->boundaries[1], val);
    this->center = (boundaries[1] + boundaries[0]) * 0.5f;
    this->extent = (boundaries[1] - boundaries[0]) * 0.5f;
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
    if(transformed)
    {
        return boundaries[0];
    }

    return !original ? boundaries[0] : original->boundaries[0];
}

const glm::vec3 &BoundingBox::MaxPoint(bool transformed) const
{
    if (transformed)
    {
        return boundaries[1];
    }

    return !original ? boundaries[1] : original->boundaries[1];
}

const glm::vec3 &BoundingBox::Center(bool transformed) const
{
    if (transformed)
    {
        return center;
    }

    return !original ? center : original->center;
}

const glm::vec3 &BoundingBox::Extent(bool transformed) const
{
    if (transformed)
    {
        return extent;
    }

    return !original ? extent : original->extent;
}

void BoundingBox::Transform(const glm::mat4x4 &model)
{
    if (!original) { original = std::make_shared<BoundingBox>(*this); }

    this->boundaries[0] = glm::vec3(model * glm::vec4(original->boundaries[0],
                                    1.0f));
    this->boundaries[1] = glm::vec3(model * glm::vec4(original->boundaries[1],
                                    1.0f));;
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