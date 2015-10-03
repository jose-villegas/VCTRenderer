#pragma once
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
        TransformMatrices();
        virtual ~TransformMatrices();

        void UpdateModelMatrix(const glm::mat4x4 &rModel);
        void UpdateViewMatrix(const glm::mat4x4 &rView);
        void UpdateProjectionMatrix(const glm::mat4x4 &rProjection);

        void RecalculateMatrices();

        void SetUniforms(oglplus::Program &program);
    private:
        Matrices matrices;

        bool modelMatrixChanged;
        bool viewMatrixChanged;
        bool projectionMatrixChanged;
        bool modelViewMatrixChanged;
};