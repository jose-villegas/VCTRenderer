#pragma once
#include "mesh.h"

class Node
{
    public:
        std::string name;

        glm::vec3 position;
        glm::vec3 scaling;
        glm::quat rotation;

        std::vector<std::shared_ptr<OGLMesh>> meshes;
        std::vector<Node> nodes;
        Node();
        virtual ~Node();

        glm::mat4 &GetTransformMatrix() const;

        // draws only the meshes associated to the node
        void Draw();
        // draws the meshes associated to the parents and all descendants
        void DrawRecursive();
};

