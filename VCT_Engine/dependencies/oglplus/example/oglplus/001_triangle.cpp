/**
 *  @example oglplus/001_triangle_glsl120.cpp
 *  @brief Shows the basic usage of OGLplus by drawing a blue triangle (using GLSL 120).
 *
 *  @oglplus_screenshot{001_triangle_glsl120}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
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
	// VBO for the triangle's vertices
	Buffer verts;
public:
	TriangleExample(void)
	{
		// Set the vertex shader source and compile it
		vs.Source(StrCRef(" \
			#version 120\n \
			attribute vec3 Position; \
			void main(void) \
			{ \
				gl_Position = vec4(Position, 1.0); \
			} \
		")).Compile();

		// set the fragment shader source and compile it
		fs.Source(StrCRef(" \
			#version 120\n \
			void main(void) \
			{ \
				gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0); \
			} \
		")).Compile();

		// attach the shaders to the program
		prog.AttachShader(vs).AttachShader(fs);
		// link and use it
		prog.Link().Use();

		// bind the VAO for the triangle
		triangle.Bind();

		GLfloat triangle_verts[9] = {
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f,-1.0f, 0.0f
		};
		// bind the VBO for the triangle vertices
		verts.Bind(Buffer::Target::Array);
		// upload the data
		// since triangle_vertices is a static array
		// it is not necessary to specify its element count
		Buffer::Data(Buffer::Target::Array, triangle_verts);
		// setup the vertex attribs array for the vertices
		(prog|"Position").Setup<Vec3f>().Enable();

		gl.ClearColor(0.5f, 0.5f, 0.5f, 0.0f);
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
