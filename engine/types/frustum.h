#pragma once

class BoundingVolume;

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
};
