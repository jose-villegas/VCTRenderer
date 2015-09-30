/**
 *  @example oglplus/002_rect.cpp
 *  @brief Shows how to draw a shaded rectangle
 *
 *  @oglplus_screenshot{002_rect}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_cpp_feat{SCOPED_ENUM_TEMPLATE_PARAMS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/all.hpp>
#include <oglplus/client_context.hpp>

#include "example.hpp"

#include <sstream>

namespace oglplus {

class RectangleExample : public Example
{
private:
	// Stateful wrapper around the current OpenGL context
	ClientContext gl;

	// Vertex and fragment shader
	Shader vs, fs;

	// Program
	Program prog;

	// A vertex array object for the rendered rectangle
	VertexArray rectangle;

	// VBOs for the rectangle's vertices and colors
	Buffer verts;
	Buffer colors;
public:
	RectangleExample(void)
	 : vs(ShaderType::Vertex)
	 , fs(ShaderType::Fragment)
	{
		// this could be any istream
		std::stringstream vs_source(
			"#version 120\n"
			"attribute vec2 Position;"
			"attribute vec3 Color;"
			"varying vec3 vertColor;"
			"void main(void)"
			"{"
			"	vertColor = Color;"
			"	gl_Position = vec4(Position, 0.0, 1.0);"
			"}"
		);
		// set the vertex shader source
		vs.Source(GLSLSource::FromStream(vs_source));
		// compile it
		vs.Compile();

		std::stringstream fs_source(
			"#version 120\n"
			"varying vec3 vertColor;"
			"void main(void)"
			"{"
			"	gl_FragColor = vec4(vertColor, 1.0);"
			"}"
		);
		// set the fragment shader source
		fs.Source(GLSLSource::FromStream(fs_source));
		// compile it
		fs.Compile();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// link it
		prog.Link();

		// and use it
		gl.Program.Bind(prog);
		// bind the VAO for the rectangle
		gl.VertexArray.Bind(rectangle);

		// bind the VBO for the rectangle vertices
		if(auto x = gl.Buffer.Array.Push(verts))
		{
			GLfloat rectangle_verts[8] = {
				-1.0f, -1.0f,
				-1.0f,  1.0f,
				 1.0f, -1.0f,
				 1.0f,  1.0f
			};
			// upload the data
			gl.Buffer.Array.Data(8, rectangle_verts);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib vert_attr(prog, "Position");
			vert_attr.Setup<Vec2f>().Enable();
		}
		// bind the VBO for the rectangle colors
		if(auto x = gl.Buffer.Array.Push(colors))
		{
			GLfloat rectangle_colors[12] = {
				1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f,
			};
			// upload the data
			gl.Buffer.Array.Data(12, rectangle_colors);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib color_attr(prog, "Color");
			color_attr.Setup<Vec3f>().Enable();
		}
		gl.Caps.DepthTest.Disable();
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport.Set(0, 0, width, height);
	}

	void Render(double)
	{
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
