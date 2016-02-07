#include <glm/detail/func_geometric.hpp>

#include "frustum.h"

#include "bounding_volume.h"

Frustum::Frustum()
{
}


Frustum::~Frustum()
{
}

void Frustum::CalculatePlanes(const glm::mat4x4 &mvMatrix)
{
    // Extract frustum planes from matrix
    // Planes are in format: normal(xyz), offset(w)
    fPlane[Right] = glm::vec4(mvMatrix[0][3] - mvMatrix[0][0],
                              mvMatrix[1][3] - mvMatrix[1][0],
                              mvMatrix[2][3] - mvMatrix[2][0],
                              mvMatrix[3][3] - mvMatrix[3][0]);
    fPlane[Left] = glm::vec4(mvMatrix[0][3] + mvMatrix[0][0],
                             mvMatrix[1][3] + mvMatrix[1][0],
                             mvMatrix[2][3] + mvMatrix[2][0],
                             mvMatrix[3][3] + mvMatrix[3][0]);
    fPlane[Bottom] = glm::vec4(mvMatrix[0][3] + mvMatrix[0][1],
                               mvMatrix[1][3] + mvMatrix[1][1],
                               mvMatrix[2][3] + mvMatrix[2][1],
                               mvMatrix[3][3] + mvMatrix[3][1]);
    fPlane[Top] = glm::vec4(mvMatrix[0][3] - mvMatrix[0][1],
                            mvMatrix[1][3] - mvMatrix[1][1],
                            mvMatrix[2][3] - mvMatrix[2][1],
                            mvMatrix[3][3] - mvMatrix[3][1]);
    fPlane[Far] = glm::vec4(mvMatrix[0][3] - mvMatrix[0][2],
                            mvMatrix[1][3] - mvMatrix[1][2],
                            mvMatrix[2][3] - mvMatrix[2][2],
                            mvMatrix[3][3] - mvMatrix[3][2]);
    fPlane[Near] = glm::vec4(mvMatrix[0][3] + mvMatrix[0][2],
                             mvMatrix[1][3] + mvMatrix[1][2],
                             mvMatrix[2][3] + mvMatrix[2][2],
                             mvMatrix[3][3] + mvMatrix[3][2]);

    // Normalize them
    for (int i = 0; i < 6; i++)
    {
        float invl = std::sqrt(fPlane[i].x * fPlane[i].x +
                               fPlane[i].y * fPlane[i].y +
                               fPlane[i].z * fPlane[i].z);
        fPlane[i] /= invl;
    }
}

bool Frustum::BoxInFrustum(const BoundingVolume &bVolume)
{
    // check box outside/inside of frustum
    for (int i = 0; i < 6; i++)
    {
        int out = 0;
        out += ((dot(fPlane[i], glm::vec4(bVolume.minPoint.x, bVolume.minPoint.y,
                                          bVolume.minPoint.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((dot(fPlane[i], glm::vec4(bVolume.maxPoint.x, bVolume.minPoint.y,
                                          bVolume.minPoint.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((dot(fPlane[i], glm::vec4(bVolume.minPoint.x, bVolume.maxPoint.y,
                                          bVolume.minPoint.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((dot(fPlane[i], glm::vec4(bVolume.maxPoint.x, bVolume.maxPoint.y,
                                          bVolume.minPoint.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((dot(fPlane[i], glm::vec4(bVolume.minPoint.x, bVolume.minPoint.y,
                                          bVolume.maxPoint.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((dot(fPlane[i], glm::vec4(bVolume.maxPoint.x, bVolume.minPoint.y,
                                          bVolume.maxPoint.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((dot(fPlane[i], glm::vec4(bVolume.minPoint.x, bVolume.maxPoint.y,
                                          bVolume.maxPoint.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((dot(fPlane[i], glm::vec4(bVolume.maxPoint.x, bVolume.maxPoint.y,
                                          bVolume.maxPoint.z, 1.0f)) < 0.0) ? 1 : 0);

        if (out == 8) { return false; }
    }

    return true;
}
