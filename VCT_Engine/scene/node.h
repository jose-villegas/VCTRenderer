#pragma once
#include "mesh.h"
class Node
{
    public:
        std::vector<Node *> childrens;
        std::vector<Mesh *> meshes;
        Node();
        virtual ~Node();
};

