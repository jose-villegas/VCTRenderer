#pragma once
#include "mesh.h"

class Node
{
    private:
        glm::mat4x4 modelMatrix;

        glm::vec3 position;
        glm::vec3 scaling;
        glm::quat rotation;
        // position, scale or rotation changed
        bool transformChanged;
    public:
        std::string name;

        std::vector<std::shared_ptr<OGLMesh>> meshes;
        std::vector<Node> nodes;
        Node();
        virtual ~Node();

        const glm::mat4x4 &RecalculateModelMatrix();

        // draws only the meshes associated to the node
        void Draw();
        // draws the meshes associated to the parents and all descendants
        void DrawRecursive();
        // transformations
        void Transform(const glm::vec3 &position,
                       const glm::vec3 &scaling,
                       const glm::quat &rotation);
        void Position(const glm::vec3 &position);
        void Scaling(const glm::vec3 &scaling);
        void Rotation(const glm::quat &rotation);
};

