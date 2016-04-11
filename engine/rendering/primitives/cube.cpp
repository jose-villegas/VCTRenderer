#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "cube.h"
#include <oglplus/vertex_attrib.hpp>

void Cube::Load()
{
    if (loaded) { return; }

    using namespace oglplus;
    // bind vao
    cube.Bind();
    //positions
    verts.Bind(BufferTarget::Array);
    {
        std::vector<float> data;
        int perVertex = cubeShape.Positions(data);
        Buffer::Data(BufferTarget::Array, data);
        VertexArrayAttrib(VertexAttribSlot(0)).Enable()
        .Pointer(perVertex, DataType::Float, false,
                 sizeof(float) * perVertex, 0);
    }
    // uvs
    texcoords.Bind(BufferTarget::Array);
    {
        std::vector<float> data;
        int perVertex = cubeShape.TexCoordinates(data);
        Buffer::Data(BufferTarget::Array, data);
        VertexArrayAttrib(VertexAttribSlot(1)).Enable()
        .Pointer(perVertex, DataType::Float, false,
                 sizeof(float) * perVertex, 0);
    }
    //positions
    normals.Bind(BufferTarget::Array);
    {
        std::vector<float> data;
        int perVertex = cubeShape.Normals(data);
        Buffer::Data(BufferTarget::Array, data);
        VertexArrayAttrib(VertexAttribSlot(2)).Enable()
        .Pointer(perVertex, DataType::Float, false,
                 sizeof(float) * perVertex, 0);
    }
}

void Cube::DrawElements() const
{
    cubeInstructions.Draw(cubeIndices);
}

Cube::Cube() : cubeInstructions(cubeShape.Instructions()),
    cubeIndices(cubeShape.Indices())
{
}

Cube::~Cube()
{
}
