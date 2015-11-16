#pragma once

#include <glm/mat4x4.hpp>

class Frustum;

struct Matrices
{
    glm::mat4x4 modelView;
    glm::mat4x4 modelViewProjection;
    glm::mat4x4 model;
    glm::mat4x4 view;
    glm::mat4x4 projection;
    glm::mat4x4 normal;
};

class TransformMatrices
{
    public:
        enum MatrixId
        {
            ModelView,
            ModelViewProjection,
            Model,
            View,
            Projection,
            Normal,
            MATRIX_ID_MAX
        };

        TransformMatrices();
        virtual ~TransformMatrices();

        void UpdateModelMatrix(const glm::mat4x4 &rModel);
        void UpdateViewMatrix(const glm::mat4x4 &rView);
        void UpdateProjectionMatrix(const glm::mat4x4 &rProjection);

        void RecalculateMatrices(bool useInverseTranspose = false);
        // passes viewProjection matrix to calculate frustum planes
        void UpdateFrustumPlanes(Frustum &fUpdate);
        // access to matrices
        const glm::mat4x4 &GetModelView() const { return matrices.modelView; };
        const glm::mat4x4 &GetModelViewProjection() const { return matrices.modelViewProjection; };
        const glm::mat4x4 &GetModel() const { return matrices.model; };
        const glm::mat4x4 &GetView() const { return matrices.view; };
        const glm::mat4x4 &GetProjection() const { return matrices.projection; };
        const glm::mat4x4 &GetNormal() const { return matrices.normal; };
    private:
        Matrices matrices;

        bool modelMatrixChanged;
        bool viewMatrixChanged;
        bool projectionMatrixChanged;
        bool modelViewMatrixChanged;
};