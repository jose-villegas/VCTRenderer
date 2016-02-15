#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <oglplus/vertex_attrib.hpp>
#include <oglplus/context.hpp>
#include "mesh.h"

#include "../types/vertex.h"

Mesh::Mesh() : name("Default Mesh")
{
}

Mesh::~Mesh()
{
}

void Mesh::FreeRawData()
{
    this->vertices.clear();
    this->indices.clear();
}

OGLMesh::OGLMesh() : loaded(false)
{
}

OGLMesh::~OGLMesh()
{
}

void OGLMesh::Load()
{
    if (oglElementArrayBuffer || oglArrayBuffer) { return; }

    using namespace oglplus;
    this->oglVertexArray = std::make_unique<VertexArray>();
    oglVertexArray->Bind();
    // create vertex buffer object and upload vertex data
    oglArrayBuffer = std::make_unique<Buffer>();
    oglArrayBuffer->Bind(Buffer::Target::Array);
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
    oglElementArrayBuffer = std::make_unique<Buffer>();
    oglElementArrayBuffer->Bind(Buffer::Target::ElementArray);
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

void OGLMesh::BindArrayBuffer() const
{
    this->oglArrayBuffer->Bind(oglplus::Buffer::Target::Array);
}

void OGLMesh::BindElementArrayBuffer() const
{
    this->oglElementArrayBuffer->Bind(oglplus::Buffer::Target::ElementArray);
}

void OGLMesh::BindVertexArrayObject() const
{
    this->oglVertexArray->Bind();
}

void OGLMesh::DrawElements() const
{
    static oglplus::Context gl;
    gl.DrawElements(oglplus::PrimitiveType::Triangles, indicesCount,
                    oglplus::DataType::UnsignedInt);
}