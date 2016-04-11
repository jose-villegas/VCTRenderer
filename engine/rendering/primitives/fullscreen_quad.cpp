#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "fullscreen_quad.h"

#include <oglplus/vertex_attrib.hpp>
#include <oglplus/context.hpp>

void FullscreenQuad::Load()
{
    using namespace oglplus;

    if (loaded) return;

    // data for fs quad
    static const std::array<float, 20> fsQuadVertexBufferData =
    {
        // X    Y    Z     U     V
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // vertex 0
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // vertex 1
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // vertex 2
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // vertex 3
    };
    // data for element buffer array
    static const std::array<unsigned int, 6> indexData =
    {
        0, 1, 2, // first triangle
        2, 1, 3, // second triangle
    };
    // bind vao for full screen quad
    fsQuadVertexArray.Bind();
    fsQuadVertexBuffer.Bind(Buffer::Target::Array);
    Buffer::Data(Buffer::Target::Array, fsQuadVertexBufferData);
    // set up attrib points
    VertexArrayAttrib(VertexAttribSlot(0)).Enable() // position
    .Pointer(3, DataType::Float, false, 5 * sizeof(float),
             reinterpret_cast<const GLvoid *>(0));
    VertexArrayAttrib(VertexAttribSlot(1)).Enable() // uvs
    .Pointer(2, DataType::Float, false, 5 * sizeof(float),
             reinterpret_cast<const GLvoid *>(12));
    // bind and fill element array
    fsQuadElementBuffer.Bind(Buffer::Target::ElementArray);
    Buffer::Data(Buffer::Target::ElementArray, indexData);
    // unbind vao
    NoVertexArray().Bind();
    // mesh loaded
    loaded = true;
}

void FullscreenQuad::DrawElements() const
{
    static oglplus::Context gl;
    fsQuadVertexArray.Bind();
    gl.DrawElements(
        oglplus::PrimitiveType::Triangles, 6,
        oglplus::DataType::UnsignedInt
    );
}

FullscreenQuad::FullscreenQuad()
{
}

FullscreenQuad::~FullscreenQuad()
{
}
