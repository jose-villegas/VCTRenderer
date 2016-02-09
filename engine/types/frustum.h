#pragma once
#include <array>

#include <glm\detail\type_mat4x4.hpp>
#include <glm\detail\type_vec4.hpp>

class BoundingBox;
class BoundingSphere;
class BoundingBox;

class Frustum
{
    public:
        void ExtractPlanes(const glm::mat4x4 &mvMatrix, bool normalize = true);
    protected:
        enum Plane
        {
            Left, Right,
            Bottom, Top,
            Near, Far,
        };

        std::array<glm::vec4, 6> planes;
};

class CullingFrustum : public Frustum
{
    public:
        CullingFrustum();
        virtual ~CullingFrustum();

        bool InFrustum(const BoundingBox &volume) const;
};
