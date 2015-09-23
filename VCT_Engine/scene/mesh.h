#pragma once
#include "material.h"
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

struct Face
{
    std::array <std::shared_ptr<Vertex>, 3> vertices;
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
        std::shared_ptr<OGLMaterial> material;

        Mesh();
        virtual ~Mesh();

        void FreeRawData();
};

class OGLMesh : public Mesh
{
    protected:
        static oglplus::Context gl;

        std::unique_ptr<oglplus::Buffer> oglArrayBuffer;
        std::unique_ptr<oglplus::Buffer> oglElementArrayBuffer;
        std::unique_ptr<oglplus::VertexArray> oglVertexArray;
        bool onGPUMemory;
        unsigned int indicesCount;
        unsigned int vertexCount;
    public:

        OGLMesh();
        virtual ~OGLMesh();

        void UploadToGPU(bool unloadFromRAM = true);
        void BindArrayBuffer();
        void BindElementArrayBuffer();
        void BindVertexArrayObject();

        void BufferPointers(oglplus::Program &program);
        void BufferSetup(oglplus::Program &program);

        void DrawElements();
        bool OnGPUMemory() const { return onGPUMemory; }
    private:
        // No copying or copy assignment allowed of this class or any derived class
        OGLMesh(OGLMesh const &);
        OGLMesh &operator=(OGLMesh const &);

};

