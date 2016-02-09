#pragma once

class BoundingSphere;
class BoundingVolume;

class Frustum
{
    public:
        void CalculatePlanes(const glm::mat4x4 &mvMatrix, bool normalize = false);
    protected:
        enum Plane
        {
            Left, Right,
            Bottom, Top,
            Near, Far,
        };

        glm::vec4 planes[6];

};

class CullingFrustum : public Frustum
{
    public:
        CullingFrustum();
        virtual ~CullingFrustum();

        bool BoxInFrustum(const BoundingVolume &volume) const;
        bool SphereInFrustum(const BoundingSphere &volume) const;
};
