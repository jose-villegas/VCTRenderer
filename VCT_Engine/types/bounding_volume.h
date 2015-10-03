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

inline BoundingVolume operator*(const BoundingVolume &lhs,
                                const glm::mat4 &rhs)
{
    // copy source values
    BoundingVolume result = lhs;
    // transform points
    result.maxPoint = glm::vec3(glm::vec4(result.maxPoint, 1.0f) * rhs);
    result.minPoint = glm::vec3(glm::vec4(result.minPoint, 1.0f) * rhs);
    // result modified b volume
    return result;
}