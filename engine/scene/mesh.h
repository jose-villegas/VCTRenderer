#pragma once

#include "../types/bounding_box.h"

#include <vector>
#include <oglplus/buffer.hpp>
#include <oglplus/vertex_array.hpp>

class OGLMaterial;
struct Vertex;

class Mesh
{
    public:
        // node boundaries
        BoundingBox boundaries;
        // mesh identifier
        std::string name;
        // mesh raw data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        // material used by mesh
        std::shared_ptr<OGLMaterial> material;

        Mesh();
        ~Mesh();

        void FreeRawData();
};

class OGLMesh : public Mesh
{
    protected:
        std::unique_ptr<oglplus::Buffer> oglArrayBuffer;
        std::unique_ptr<oglplus::Buffer> oglElementArrayBuffer;
        std::unique_ptr<oglplus::VertexArray> oglVertexArray;

        bool loaded;
        unsigned int indicesCount;
        unsigned int vertexCount;
    public:

        OGLMesh();
        ~OGLMesh();

        void Load();
        void BindArrayBuffer() const;
        void BindElementArrayBuffer() const;
        void BindVertexArrayObject() const;

        void DrawElements() const;
        bool IsLoaded() const { return loaded; }
    protected:
        // No copying or copy assignment allowed of this class or any derived class
        OGLMesh(OGLMesh const &) = delete;
        OGLMesh &operator=(OGLMesh const &) = delete;
};

