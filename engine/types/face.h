#pragma once

#include <array>
#include <memory>
#include <glm/detail/type_vec3.hpp>

struct Vertex;

class Face
{
        std::array<std::shared_ptr<Vertex>, 3> vertices;
        std::array<unsigned int, 3> indices;
        glm::vec3 normal;

        Face() : normal(glm::vec3(0)) {};
        virtual ~Face() {};
};