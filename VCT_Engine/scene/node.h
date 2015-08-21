#pragma once
#include "mesh.h"

class Node
{
    public:
        std::string name;

        glm::mat4 transformation;
        std::vector<Mesh *> meshes;
        std::vector<Node> nodes;
        Node();
        virtual ~Node();
};

