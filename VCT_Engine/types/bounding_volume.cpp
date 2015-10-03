#include "stdafx.h"
#include "bounding_volume.h"


BoundingVolume::BoundingVolume()
{
    this->maxPoint = glm::vec3(std::numeric_limits<float>::lowest());
    this->minPoint = glm::vec3(std::numeric_limits<float>::infinity());
}


BoundingVolume::~BoundingVolume()
{
}

void BoundingVolume::TryMin(const glm::vec3 &vMin)
{
    minPoint.x > vMin.x ? minPoint.x = vMin.x : 0;
    minPoint.y > vMin.y ? minPoint.y = vMin.y : 0;
    minPoint.z > vMin.z ? minPoint.z = vMin.z : 0;
}

void BoundingVolume::TryMax(const glm::vec3 &vMax)
{
    maxPoint.x < vMax.x ? maxPoint.x = vMax.x : 0;
    maxPoint.y < vMax.y ? maxPoint.y = vMax.y : 0;
    maxPoint.z < vMax.z ? maxPoint.z = vMax.z : 0;
}

void BoundingVolume::TryMinMax(const glm::vec3 &vMin, const glm::vec3 &vMax)
{
    TryMin(vMin); TryMax(vMax);
}
