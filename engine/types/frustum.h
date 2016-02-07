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
        bool VolumeInFrustum(const BoundingVolume &bVolume) const;
    private:
        glm::vec4 fPlane[6];
};
