#pragma once
#include "material.h"
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 uv;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    Vertex();
    virtual ~Vertex();
    void Orthogonalize();
};

struct Face
{
    std::array<Vertex *, 3> vertices;
    std::array<unsigned int, 3> indices;
    glm::vec3 normal;

    Face();
    virtual ~Face();
};

class Mesh
{
    public:
        Mesh();
        virtual ~Mesh();
    private:
        std::vector<Face> faces;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        // material used by mesh
        Material * material;
};

