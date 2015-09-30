/**
 *  @example oglplus/002_triangle.cpp
 *  @brief Shows the basic usage of OGLplus by drawing a simple triangle.
 *
 *  @oglplus_screenshot{002_triangle}
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include "example.hpp"

namespace oglplus {

class TriangleExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader
	VertexShader vs;

	// Fragment shader
	FragmentShader fs;

	// Program
	Program prog;

	// A vertex array object for the rendered triangle
	VertexArray triangle;

	// VBOs for the triangle's vertices and colors
	Buffer verts;
	Buffer colors;
public:
	TriangleExample(void)
	{
		try
		{
			// Set the vertex shader source
			vs.Source(" \
				#version 330\n \
				in vec3 Position; \
				in vec3 Color; \
				out vec4 vertColor; \
				void main(void) \
				{ \
					gl_Position = vec4(Position, 1.0); \
					vertColor = vec4(Color, 1.0); \
				} \
			");
			// compile it
			vs.Compile();
		}
		catch(CompileError&)
		{
			// Set the fallback vertex shader source
			vs.Source(" \
				#version 120\n \
				attribute vec3 Position; \
				attribute vec3 Color; \
				varying vec4 vertColor; \
				void main(void) \
				{ \
					gl_Position = vec4(Position, 1.0); \
					vertColor = vec4(Color, 1.0); \
				} \
			");
			// compile it
			vs.Compile();
		}

		try
		{
			fs.Source(" \
				#version 330\n \
				in vec4 vertColor; \
				out vec4 fragColor; \
				void main(void) \
				{ \
					fragColor = vertColor; \
				} \
			");
			fs.Compile();
		}
		catch(CompileError&)
		{
			fs.Source(" \
				#version 120\n \
				varying vec4 vertColor; \
				void main(void) \
				{ \
					gl_FragColor = vertColor; \
				} \
			");
			fs.Compile();
		}

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the triangle
		triangle.Bind();

		GLfloat triangle_verts[9] = {
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};
		// bind the VBO for the triangle vertices
		verts.Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(Buffer::Target::Array, 9, triangle_verts);
		// setup the vertex attribs array for the vertices
		VertexArrayAttrib(prog, "Position").Setup<GLfloat>(3).Enable();
		//

		GLfloat triangle_colors[9] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};
		// bind the VBO for the triangle colors
		colors.Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(Buffer::Target::Array, 9, triangle_colors);
		// setup the vertex attribs array
		VertexArrayAttrib(prog, "Color").Setup<GLfloat>(3).Enable();

		gl.ClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		gl.Disable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
	}

	void Render(double)
	{
		gl.Clear().ColorBuffer();

		gl.DrawArrays(PrimitiveType::Triangles, 0, 3);
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
	return std::unique_ptr<Example>(new TriangleExample);
}

} // namespace oglplus
