#pragma once
class BoundingVolume
{
    public:
        glm::vec3 minPoint;
        glm::vec3 maxPoint;

        BoundingVolume();
        virtual ~BoundingVolume();

        // tries to set new min or max value
        void TryMin(const glm::vec3 &vMin);
        void TryMax(const glm::vec3 &vMax);
        void TryMinMax(const glm::vec3 &vMin, const glm::vec3 &vMax);
};

