#pragma once
#include "mesh.h"

class Node
{
    public:
        std::string name;

        glm::vec3 position;
        glm::vec3 scaling;
        glm::quat rotation;

        std::vector<Mesh *> meshes;
        std::vector<Node> nodes;
        Node();
        virtual ~Node();

        glm::mat4 &GetTransformMatrix() const;
};

