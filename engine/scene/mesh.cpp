#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <oglplus/vertex_attrib.hpp>
#include <oglplus/context.hpp>
#include "mesh.h"

#include "../types/vertex.h"

Mesh::Mesh()
{
    name = "Default Mesh";
}

Mesh::~Mesh()
{
}

void Mesh::FreeRawData()
{
    this->vertices.clear();
    this->indices.clear();
}

MeshDrawer::MeshDrawer() : loaded(false)
{
}

MeshDrawer::~MeshDrawer()
{
}

void MeshDrawer::Load()
{
    if (elementBuffer || vertexBuffer) { return; }

    using namespace oglplus;
    this->vertexArray = std::make_unique<VertexArray>();
    vertexArray->Bind();
    // create vertex buffer object and upload vertex data
    vertexBuffer = std::make_unique<Buffer>();
    vertexBuffer->Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, this->vertices);
    // setup vertex data interleaving
    VertexArrayAttrib(VertexAttribSlot(0)).Enable() // position
    .Pointer(3, DataType::Float, false, sizeof(Vertex),
             reinterpret_cast<const GLvoid *>(0));
    VertexArrayAttrib(VertexAttribSlot(1)).Enable() // uvs
    .Pointer(3, DataType::Float, false, sizeof(Vertex),
             reinterpret_cast<const GLvoid *>(12));
    VertexArrayAttrib(VertexAttribSlot(2)).Enable() // normals
    .Pointer(3, DataType::Float, false, sizeof(Vertex),
             reinterpret_cast<const GLvoid *>(24));
    VertexArrayAttrib(VertexAttribSlot(3)).Enable() // tangents
    .Pointer(3, DataType::Float, false, sizeof(Vertex),
             reinterpret_cast<const GLvoid *>(36));
    VertexArrayAttrib(VertexAttribSlot(4)).Enable() // bitangents
    .Pointer(3, DataType::Float, false, sizeof(Vertex),
             reinterpret_cast<const GLvoid *>(48));
    // create element (indices) buffer object and upload data
    elementBuffer = std::make_unique<Buffer>();
    elementBuffer->Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, this->indices);
    // save number of faces and vertices for rendering
    this->indicesCount = static_cast<unsigned int>(this->indices.size());
    this->vertexCount = static_cast<unsigned int>(this->vertices.size());
    this->vertices.clear();
    this->indices.clear();
    // unbind vao
    NoVertexArray().Bind();
    loaded = true;
}

void MeshDrawer::BindArrayBuffer() const
{
    this->vertexBuffer->Bind(oglplus::Buffer::Target::Array);
}

void MeshDrawer::BindElementArrayBuffer() const
{
    this->elementBuffer->Bind(oglplus::Buffer::Target::ElementArray);
}

void MeshDrawer::BindVertexArrayObject() const
{
    this->vertexArray->Bind();
}

void MeshDrawer::DrawElements() const
{
    static oglplus::Context gl;
    gl.DrawElements(oglplus::PrimitiveType::Triangles, indicesCount,
                    oglplus::DataType::UnsignedInt);
}