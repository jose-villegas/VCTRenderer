/**
 *  @example oglplus/002_shader_lit.cpp
 *  @brief Shows how to draw a shaded rectangle
 *
 *  @oglplus_screenshot{002_shader_lit}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_cpp_feat{USER_DEFINED_LITERALS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/opt/shader_literals.hpp>

#include "example.hpp"

namespace oglplus {

class RectangleExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	// Program
	Program prog;

	// A vertex array object for the rendered rectangle
	VertexArray rectangle;

	// VBOs for the rectangle's vertices and colors
	Buffer verts;
	Buffer colors;
public:
	RectangleExample(void)
	{
		prog <<	"#version 120\n"
			"attribute vec2 Position;"
			"attribute vec3 Color;"
			"varying vec3 vertColor;"
			"void main(void)"
			"{"
			"	vertColor = Color;"
			"	gl_Position = vec4(Position, 0.0, 1.0);"
			"}"_glsl_vs;

		prog <<	"#version 120\n"
			"varying vec3 vertColor;"
			"void main(void)"
			"{"
			"	gl_FragColor = vec4(vertColor, 1.0);"
			"}"_glsl_fs;

		prog.Link();
		prog.Use();

		// bind the VAO for the rectangle
		rectangle.Bind();

		GLfloat rectangle_verts[8] = {
			-1.0f, -1.0f,
			-1.0f,  1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f
		};
		// bind the VBO for the rectangle vertices
		verts.Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(Buffer::Target::Array, 8, rectangle_verts);
		// setup the vertex attribs array for the vertices
		VertexArrayAttrib vert_attr(prog, "Position");
		vert_attr.Setup<Vec2f>();
		vert_attr.Enable();

		GLfloat rectangle_colors[12] = {
			0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
		};
		// bind the VBO for the rectangle colors
		colors.Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(Buffer::Target::Array, 12, rectangle_colors);
		// setup the vertex attribs array for the vertices
		VertexArrayAttrib color_attr(prog, "Color");
		color_attr.Setup<GLfloat>(3);
		color_attr.Enable();
		//

		gl.Disable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
	}

	void Render(double)
	{
		gl.Clear().ColorBuffer();

		gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);
	}
};

void setupExample(ExampleParams& /*params*/){ }

std::unique_ptr<ExampleThread> makeExampleThread(
	Example& /*example*/,
	unsigned /*thread_id*/,
	const ExampleParams& /*params*/
){ return std::unique_ptr<ExampleThread>(); }

std::unique_ptr<Example> makeExample(const ExampleParams& /*params*/)
{
	return std::unique_ptr<Example>(new RectangleExample);
}

} // namespace oglplus
