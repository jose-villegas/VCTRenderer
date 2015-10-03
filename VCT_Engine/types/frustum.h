#pragma once
#include "bounding_volume.h"

class Frustum
{

    public:
        enum Plane
        {
            Right = 0,
            Left,
            Bottom,
            Top,
            Far,
            Near
        };

        Frustum();
        virtual ~Frustum();

        void CalculatePlanes(const glm::mat4x4 &mvMatrix);
        bool BoxInFrustum(const BoundingVolume &bVolume);
    private:
        glm::vec4 fPlane[6];

        inline int HalfPlaneTest(const glm::vec3 &p, const glm::vec3 &normal,
                                 float offset);
};

