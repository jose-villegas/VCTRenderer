#include <glm/gtc/matrix_inverse.hpp>

#include "transform_matrices.h"

#include "frustum.h"

TransformMatrices::TransformMatrices() : matrices(), matricesChanged(None)
{
}

TransformMatrices::~TransformMatrices()
{
}

void TransformMatrices::UpdateModelMatrix(const glm::mat4x4 &rModel)
{
    if (rModel == matrices.model) { return; }

    matrices.model = rModel;
    matricesChanged = static_cast<Changed>(matricesChanged | ModelChanged);
}

void TransformMatrices::UpdateViewMatrix(const glm::mat4x4 &rView)
{
    if (rView == matrices.model) { return; }

    matrices.view = rView;
    matricesChanged = static_cast<Changed>(matricesChanged | ViewChanged);
}

void TransformMatrices::UpdateProjectionMatrix(const glm::mat4x4 &rProjection)
{
    if (rProjection == matrices.projection) { return; }

    matrices.projection = rProjection;
    matricesChanged = static_cast<Changed>(matricesChanged | ProjectionChanged);
}

void TransformMatrices::RecalculateMatrices(bool useInverseTranspose)
{
    if (matricesChanged & ViewChanged || matricesChanged & ModelChanged)
    {
        matrices.modelView = matrices.view * matrices.model;
    }

    if (matricesChanged & ViewChanged || matricesChanged & ModelChanged
        || matricesChanged & ProjectionChanged)
    {
        matrices.modelViewProjection = matrices.projection * matrices.modelView;
    }

    matrices.normal = useInverseTranspose
                      ? inverseTranspose(matrices.modelView)
                      : matrices.modelView;
    matricesChanged = None;
}

void TransformMatrices::UpdateFrustumPlanes(Frustum &fUpdate)
{
    // can recalculate frustum in this case
    fUpdate.CalculatePlanes(matrices.projection * matrices.view);
}
