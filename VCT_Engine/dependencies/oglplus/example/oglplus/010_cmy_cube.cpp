/**
 *  @example oglplus/010_cmy_cube_glsl120.cpp
 *  @brief Shows how to draw a RGB colored cube
 *
 *  @oglplus_screenshot{010_cmy_cube_glsl120}
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

class CubeExample : public Example
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

	// A vertex array object for the rendered cube
	VertexArray cube;

	// VBOs for the cube's vertices and normals
	Buffer verts;
	Buffer normals;
public:
	CubeExample(void)
	{
		// Set the vertex shader source and compile it
		vs.Source(
			"#version 120\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"attribute vec4 Position;"
			"attribute vec3 Normal;"
			"varying vec3 vertNormal;"
			"void main(void)"
			"{"
			"	vertNormal = Normal;"
			"	gl_Position = ProjectionMatrix *"
			"		CameraMatrix *"
			"		Position;"
			"}"
		).Compile();

		// set the fragment shader source
		// (uses the absolute value of normal as color)
		fs.Source(
			"#version 120\n"
			"varying vec3 vertNormal;"
			"void main(void)"
			"{"
			"	gl_FragColor = vec4(normalize(abs(vec3(1, 1, 1) - vertNormal)), 1.0);"
			"}"
		).Compile();

		// attach the shaders to the program
		prog.AttachShader(vs).AttachShader(fs);
		// link and use it
		prog.Link().Use();

		// bind the VAO for the cube
		cube.Bind();

		const GLfloat c[8][3] = {
			{-0.5f, -0.5f, +0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, +0.5f, -0.5f},
			{-0.5f, +0.5f, +0.5f},
			{+0.5f, -0.5f, +0.5f},
			{+0.5f, -0.5f, -0.5f},
			{+0.5f, +0.5f, -0.5f},
			{+0.5f, +0.5f, +0.5f}
		};
		const GLuint vertex_count = 6 * 2 * 3;
		const GLfloat cube_vertices[vertex_count * 3] = {
			c[0][0], c[0][1], c[0][2],
			c[2][0], c[2][1], c[2][2],
			c[1][0], c[1][1], c[1][2],
			c[0][0], c[0][1], c[0][2],
			c[3][0], c[3][1], c[3][2],
			c[2][0], c[2][1], c[2][2],

			c[0][0], c[0][1], c[0][2],
			c[1][0], c[1][1], c[1][2],
			c[4][0], c[4][1], c[4][2],
			c[1][0], c[1][1], c[1][2],
			c[5][0], c[5][1], c[5][2],
			c[4][0], c[4][1], c[4][2],

			c[1][0], c[1][1], c[1][2],
			c[2][0], c[2][1], c[2][2],
			c[5][0], c[5][1], c[5][2],
			c[2][0], c[2][1], c[2][2],
			c[6][0], c[6][1], c[6][2],
			c[5][0], c[5][1], c[5][2],

			c[4][0], c[4][1], c[4][2],
			c[5][0], c[5][1], c[5][2],
			c[6][0], c[6][1], c[6][2],
			c[4][0], c[4][1], c[4][2],
			c[6][0], c[6][1], c[6][2],
			c[7][0], c[7][1], c[7][2],

			c[2][0], c[2][1], c[2][2],
			c[3][0], c[3][1], c[3][2],
			c[7][0], c[7][1], c[7][2],
			c[2][0], c[2][1], c[2][2],
			c[7][0], c[7][1], c[7][2],
			c[6][0], c[6][1], c[6][2],

			c[0][0], c[0][1], c[0][2],
			c[4][0], c[4][1], c[4][2],
			c[3][0], c[3][1], c[3][2],
			c[3][0], c[3][1], c[3][2],
			c[4][0], c[4][1], c[4][2],
			c[7][0], c[7][1], c[7][2]
		};
		// bind the VBO for the cube vertices
		verts.Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(
			Buffer::Target::Array,
			vertex_count * 3,
			cube_vertices
		);
		// setup the vertex attribs array for the vertices
		VertexArrayAttrib vert_attr(prog, "Position");
		vert_attr.Setup<Vec3f>();
		vert_attr.Enable();

		const GLfloat n[6][3] = {
			{-1.0f,  0.0f,  0.0f},
			{ 0.0f, -1.0f,  0.0f},
			{ 0.0f,  0.0f, -1.0f},
			{ 1.0f,  0.0f,  0.0f},
			{ 0.0f,  1.0f,  0.0f},
			{ 0.0f,  0.0f,  1.0f}
		};
		GLfloat cube_normals[vertex_count * 3];
		for(GLuint f=0;f!=6;++f)
			for(GLuint v=0;v!=6;++v)
				for(GLuint ci=0;ci!=3;++ci)
					cube_normals[(f*6+v)*3+ci] = n[f][ci];
		// bind the VBO for the cube normals
		normals.Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(
			Buffer::Target::Array,
			vertex_count * 3,
			cube_normals
		);
		// setup the vertex attribs array for the vertices
		VertexArrayAttrib normal_attr(prog, "Normal");
		normal_attr.Setup<Vec3f>();
		normal_attr.Enable();
		//
		// set the matrix for camera at (1,1,1) looking at origin
		Uniform<Mat4f>(prog, "CameraMatrix").Set(
			CamMatrixf::LookingAt(
				Vec3f(2.0f, 2.0f, 2.0f),
				Vec3f()
			)
		);
		//
		gl.ClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		Uniform<Mat4f>(prog, "ProjectionMatrix").Set(
			CamMatrixf::PerspectiveX(
				Degrees(48),
				GLfloat(width)/height,
				1, 100
			)
		);
	}

	void Render(double)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		gl.DrawArrays(PrimitiveType::Triangles, 0, 6 * 2 * 3);
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
	return std::unique_ptr<Example>(new CubeExample);
}

} // namespace oglplus
