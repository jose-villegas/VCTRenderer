#pragma once
#include "material.h"
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    Vertex() : position(glm::vec3(0)), normal(glm::vec3(0)), uv(glm::vec2(0)),
        tangent(glm::vec3(0)), bitangent(glm::vec3(0)) {};
    virtual ~Vertex() {};
    void Orthonormalize();
};

struct Face
{
    std::array<Vertex *, 3> vertices;
    std::array<unsigned int, 3> indices;
    glm::vec3 normal;

    Face() : normal(glm::vec3(0)) {};
    virtual ~Face() {};
};

class Mesh
{
    public:
        std::string name;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // material used by mesh
        Material * material;

        Mesh();
        virtual ~Mesh();

        void FreeRawData();
};

class OGLMesh : public Mesh
{
    protected:
        oglplus::Framebuffer oglFramebuffer;

        bool onGPUMemory;
    public:
        OGLMesh();
        virtual ~OGLMesh();

        GLuint UploadToGPU();
    private:
        // No copying or copy assignment allowed of this class or any derived class
        OGLMesh(OGLMesh const &);
        OGLMesh &operator=(OGLMesh const &);

};

