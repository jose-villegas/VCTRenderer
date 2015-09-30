/**
 *  @example oglplus/003_triangle.cpp
 *  @brief Shows the basic usage of OGLplus by drawing a simple transformed triangle.
 *
 *  @oglplus_screenshot{003_triangle}
 *
 *  Copyright 2008-2013 Matus Chochlik. Distributed under the Boost
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

	// Array of VBOs [0] -> vertex coords, [1] -> vertex colors
	Array<Buffer> buffs;
public:
	TriangleExample(void)
	 : buffs(2)
	{
		// Set and compile the vertex shader source
		vs.Source(" \
			#version 330\n \
			in vec3 Position; \
			in vec3 Color; \
			uniform mat4 Matrix; \
			out vec4 vertColor; \
			void main(void) \
			{ \
				gl_Position = Matrix * vec4(Position, 1.0); \
				vertColor = vec4(Color, 1.0); \
			} \
		").Compile(std::nothrow).DoneWithoutError() || vs.Source(" \
			#version 120\n \
			attribute vec3 Position; \
			attribute vec3 Color; \
			uniform mat4 Matrix; \
			varying vec4 vertColor; \
			void main(void) \
			{ \
				gl_Position = Matrix * vec4(Position, 1.0); \
				vertColor = vec4(Color, 1.0); \
			} \
		").Compile(std::nothrow).Done();

		// set and compile the fragment shader source
		fs.Source(" \
			#version 330\n \
			in vec4 vertColor; \
			out vec4 fragColor; \
			void main(void) \
			{ \
				fragColor = vertColor; \
			} \
		").Compile(std::nothrow).DoneWithoutError() || fs.Source(" \
			#version 120\n \
			varying vec4 vertColor; \
			void main(void) \
			{ \
				gl_FragColor = vertColor; \
			} \
		").Compile(std::nothrow).Done();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link().Use();

		// bind the VAO for the triangle
		triangle.Bind();

		GLfloat triangle_verts[9] = {
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};
		// bind the VBO for the triangle vertices
		buffs[0].Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(Buffer::Target::Array, triangle_verts);
		// setup the vertex attribs array for the vertices
		VertexArrayAttrib vert_attr(prog, "Position");
		vert_attr.Setup<Vec3f>();
		vert_attr.Enable();
		//

		GLfloat triangle_colors[9] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};
		// bind the VBO for the triangle colors
		buffs[1].Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(Buffer::Target::Array, triangle_colors);
		// setup the vertex attribs array
		VertexArrayAttrib color_attr(prog, "Color");
		color_attr.Setup<Vec3f>();
		color_attr.Enable();

		// set the transformation matrix
		Uniform<Mat4f> u1(prog, "Matrix");
		u1.Set(Mat4f(
			 Vec4f(2.0f,  0.0f,  0.0f, -1.0f),
			 Vec4f(0.0f,  2.0f,  0.0f, -1.0f),
			 Vec4f(0.0f,  0.0f,  2.0f,  0.0f),
			 Vec4f(0.0f,  0.0f,  0.0f,  1.0f)
		));

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
