#include <glm/detail/func_geometric.hpp>

#include "frustum.h"

#include "bounding_box.h"
#include <glm/gtc/matrix_access.hpp>

CullingFrustum::CullingFrustum()
{
}


CullingFrustum::~CullingFrustum()
{
}

void Frustum::ExtractPlanes(const glm::mat4x4 &modelView, bool normalize)
{
    // extract frustum planes from the modelView matrix
    // planes are in format: normal(xyz), offset(w)
    const glm::vec4 mRow[4] =
    {
        row(modelView, 0), row(modelView, 1),
        row(modelView, 2), row(modelView, 3)
    };
    planes[Left] = mRow[3] + mRow[0];
    planes[Right] = mRow[3] - mRow[0];
    planes[Bottom] = mRow[3] + mRow[1];
    planes[Top] = mRow[3] - mRow[1];
    planes[Near] = mRow[3] + mRow[2];
    planes[Far] = mRow[3] - mRow[2];

    if (!normalize) { return; }

    // normalize them
    for (auto &p : planes)
    {
        p /= length(p);
    }
}

const glm::vec4 &Frustum::Plane(const FrustumPlane face) const
{
    return planes[face];
}

const std::array<glm::vec4, 6> &Frustum::Planes() const
{
    return planes;
}

bool CullingFrustum::InFrustum(const BoundingBox &volume) const
{
    glm::vec3 normal;

    for (auto &plane : planes)
    {
        normal = glm::vec3(plane);
        auto d = dot(volume.Extent(true), abs(normal));
        auto r = dot(volume.Center(true), normal) + plane.w;

        if (d + r > 0.0f == false)
        {
            return false;
        }
    }

    return true;
}


