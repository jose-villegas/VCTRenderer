#pragma once

#include <glm/detail/type_vec3.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 uv;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    Vertex();
    void Orthonormalize();
};

