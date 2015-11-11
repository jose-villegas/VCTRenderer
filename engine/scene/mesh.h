#pragma once
#include "material.h"
#include "types\vertex.h"
#include "types\bounding_volume.h"

class Mesh
{

    public:
        // node boundaries
        BoundingVolume boundaries;
        // mesh identifier
        std::string name;
        // mesh raw data
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

