#pragma once
#include "vertex.h"

class Face
{
        std::array <std::shared_ptr<Vertex>, 3> vertices;
        std::array<unsigned int, 3> indices;
        glm::vec3 normal;

        Face() : normal(glm::vec3(0)) {};
        virtual ~Face() {};
};