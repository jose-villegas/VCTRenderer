#include <glm/detail/func_geometric.hpp>

#include "frustum.h"

#include "bounding_volume.h"
#include <glm/gtc/matrix_access.hpp>

CullingFrustum::CullingFrustum()
{
}


CullingFrustum::~CullingFrustum()
{
}

void Frustum::CalculatePlanes(const glm::mat4x4 &matrix, bool normalize)
{
    // extract frustum planes from matrix
    // planes are in format: normal(xyz), offset(w)
    const glm::vec4 mRow[4] =
    {
        row(matrix, 0), row(matrix, 1),
        row(matrix, 2), row(matrix, 3)
    };
    planes[Left] = mRow[3] + mRow[0];
    planes[Right] = mRow[3] - mRow[0];
    planes[Bottom] = mRow[3] + mRow[1];
    planes[Top] = mRow[3] - mRow[1];
    planes[Near] = mRow[3] + mRow[2];
    planes[Far] = mRow[3] - mRow[2];

    if (!normalize) { return; }

    // Normalize them
    for (auto &p : planes)
    {
        p /= length(p);
    }
}

bool CullingFrustum::BoxInFrustum(const BoundingVolume &volume) const
{
    // check box outside/inside of frustum
    //for (int i = 0; i < 6; i++)
    //{
    //    int out = 0;
    //    out += ((dot(plane[i], glm::vec4(bVolume.MinPoint().x, bVolume.MinPoint().y,
    //                                     bVolume.MinPoint().z, 1.0f)) < 0.0) ? 1 : 0);
    //    out += ((dot(plane[i], glm::vec4(bVolume.MaxPoint().x, bVolume.MinPoint().y,
    //                                     bVolume.MinPoint().z, 1.0f)) < 0.0) ? 1 : 0);
    //    out += ((dot(plane[i], glm::vec4(bVolume.MinPoint().x, bVolume.MaxPoint().y,
    //                                     bVolume.MinPoint().z, 1.0f)) < 0.0) ? 1 : 0);
    //    out += ((dot(plane[i], glm::vec4(bVolume.MaxPoint().x, bVolume.MaxPoint().y,
    //                                     bVolume.MinPoint().z, 1.0f)) < 0.0) ? 1 : 0);
    //    out += ((dot(plane[i], glm::vec4(bVolume.MinPoint().x, bVolume.MinPoint().y,
    //                                     bVolume.MaxPoint().z, 1.0f)) < 0.0) ? 1 : 0);
    //    out += ((dot(plane[i], glm::vec4(bVolume.MaxPoint().x, bVolume.MinPoint().y,
    //                                     bVolume.MaxPoint().z, 1.0f)) < 0.0) ? 1 : 0);
    //    out += ((dot(plane[i], glm::vec4(bVolume.MinPoint().x, bVolume.MaxPoint().y,
    //                                     bVolume.MaxPoint().z, 1.0f)) < 0.0) ? 1 : 0);
    //    out += ((dot(plane[i], glm::vec4(bVolume.MaxPoint().x, bVolume.MaxPoint().y,
    //                                     bVolume.MaxPoint().z, 1.0f)) < 0.0) ? 1 : 0);
    //    if (out == 8) { return false; }
    //}
    return true;
}

bool CullingFrustum::SphereInFrustum(const BoundingSphere &volume) const
{
    float fDistance;

    for (auto &p : planes)
    {
        fDistance = distance(glm::vec3(p), volume.Center());

        if (fDistance < -volume.Radius())
        {
            return false;
        }
    }

    return true;
}
