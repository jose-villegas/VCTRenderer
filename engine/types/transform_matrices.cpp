#include <glm/gtc/matrix_inverse.hpp>

#include "transform_matrices.h"

#include "frustum.h"

TransformMatrices::TransformMatrices()
{
}

TransformMatrices::~TransformMatrices()
{
}

void TransformMatrices::UpdateModelMatrix(const glm::mat4x4 &rModel)
{
    if (matrices.model != rModel)
    {
        matrices.model = rModel;
        modelMatrixChanged = true;
    }
    else
    {
        modelMatrixChanged = false;
    }
}

void TransformMatrices::UpdateViewMatrix(const glm::mat4x4 &rView)
{
    if (matrices.view != rView)
    {
        matrices.view = rView;
        viewMatrixChanged = true;
    }
    else
    {
        viewMatrixChanged = false;
    }
}

void TransformMatrices::UpdateProjectionMatrix(const glm::mat4x4 &rProjection)
{
    if (matrices.projection != rProjection)
    {
        matrices.projection = rProjection;
        projectionMatrixChanged = true;
    }
    else
    {
        projectionMatrixChanged = false;
    }
}

void TransformMatrices::RecalculateMatrices(bool useInverseTranspose)
{
    if (viewMatrixChanged || modelMatrixChanged)
    {
        matrices.modelView = matrices.view * matrices.model;
        modelViewMatrixChanged = true;
    }

    if (modelViewMatrixChanged || projectionMatrixChanged)
    {
        matrices.modelViewProjection = matrices.projection * matrices.modelView;
    }

    if (modelViewMatrixChanged)
    {
        matrices.normal = useInverseTranspose
                          ? inverseTranspose(matrices.modelView)
                          : matrices.modelView;
    }
}

void TransformMatrices::UpdateFrustumPlanes(Frustum &fUpdate)
{
    // can recalculate frustum in this case
    if (viewMatrixChanged || projectionMatrixChanged)
    {
        fUpdate.CalculatePlanes(matrices.projection * matrices.view);
    }
}
