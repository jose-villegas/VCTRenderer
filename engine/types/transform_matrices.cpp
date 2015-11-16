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
    if (matricesChanged & (ViewChanged | ModelChanged))
    {
        matrices.modelView = matrices.view * matrices.model;
    }

    if (matricesChanged & (ViewChanged | ModelChanged | ProjectionChanged))
    {
        matrices.modelViewProjection = matrices.projection * matrices.modelView;
    }

    matrices.normal = useInverseTranspose
                      ? inverseTranspose(matrices.modelView)
                      : matrices.modelView;
    matricesChanged = None;
}

void TransformMatrices::UpdateFrustumPlanes(Frustum &fUpdate) const
{
    // can recalculate frustum in this case
    if (matricesChanged & (ViewChanged | ProjectionChanged))
    {
        fUpdate.CalculatePlanes(matrices.projection * matrices.view);
    }
}

const TransformMatrices::Changed &TransformMatrices::MatricesChanged() const
{
    return matricesChanged;
}