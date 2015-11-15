#pragma once
#include "mesh.h"

class Node
{
    public:
        // node boundaries
        BoundingVolume boundaries;
        // node identifier
        std::string name;
        // meshes associated and children
        std::vector<std::shared_ptr<OGLMesh>> meshes;
        std::vector<std::shared_ptr<Node>> nodes;


        Node();
        ~Node();

        glm::mat4x4 GetModelMatrix() const;
        void RecalculateModelMatrix();

        // draws only the meshes associated to the node
        void Draw();
        // draws the meshes associated to the parents and all descendants
        void DrawRecursive();
        // call drawElements per mesh
        void DrawMeshes();
        // transformations
        void Transform(const glm::vec3 &position,
                       const glm::vec3 &scaling,
                       const glm::quat &rotation);
        void Position(const glm::vec3 &position);
        void Scaling(const glm::vec3 &scaling);
        void Rotation(const glm::quat &rotation);
    private:
        glm::mat4x4 modelMatrix;

        glm::vec3 position;
        glm::vec3 scaling;
        glm::quat rotation;
        // position, scale or rotation changed
        bool transformChanged;
};

