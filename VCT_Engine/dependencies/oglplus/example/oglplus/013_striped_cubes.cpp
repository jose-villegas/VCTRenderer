/**
 *  @example oglplus/013_striped_cubes.cpp
 *  @brief Shows how to draw rotating striped cubes using the Cube builder
 *
 *  @oglplus_screenshot{013_striped_cubes}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class CubeExample : public Example
{
private:
	// helper object building cube vertex attributes
	shapes::Cube make_cube;
	// helper object encapsulating cube drawing instructions
	shapes::DrawingInstructions cube_instr;
	// indices pointing to cube primitive elements
	shapes::Cube::IndexArray cube_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader
	VertexShader vs;

	// Fragment shader
	FragmentShader fs;

	// Program
	Program prog;

	// Uniforms
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	// A vertex array object for the rendered cube
	VertexArray cube;

	// VBOs for the cube's vertices and tex-coordinates
	Buffer verts;
	Buffer texcoords;
public:
	CubeExample(void)
	 : cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	 , projection_matrix(prog)
	 , camera_matrix(prog)
	 , model_matrix(prog)
	{
		// Set the vertex shader source and compile it
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"in vec4 Position;"
			"in vec2 TexCoord;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		ModelMatrix *"
			"		Position;"
			"}"
		).Compile();

		// set the fragment shader source and compile it
		fs.Source(
			"#version 140\n"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float i = int(("
			"		vertTexCoord.x+"
			"		vertTexCoord.y "
			"	)*8) % 2;"
			"	fragColor = mix("
			"		vec4(0, 0, 0, 1),"
			"		vec4(1, 1, 0, 1),"
			"		i"
			"	);"
			"}"
		).Compile();

		// attach the shaders to the program
		prog.AttachShaders(MakeGroup(vs, fs));
		// link and use it
		prog.Link().Use();

		// initialize the uniforms
		projection_matrix.BindTo("ProjectionMatrix");
		camera_matrix.BindTo("CameraMatrix");
		model_matrix.BindTo("ModelMatrix");

		// bind the VAO for the cube
		cube.Bind();

		// bind the VBO for the cube vertices
		verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Positions(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex).Enable();
		}

		// bind the VBO for the cube UV-coordinates
		texcoords.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.TexCoordinates(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "TexCoord");
			attr.Setup<GLfloat>(n_per_vertex).Enable();
		}

		+Capability::DepthTest;
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				float(width)/height,
				1, 30
			)
		);
	}

	void Render(double time)
	{
		const GLfloat clear_color[4] = {0.8f, 0.8f, 0.7f, 0.0f};
		gl.ClearColorBuffer(0, clear_color);
		gl.ClearDepthBuffer(1.0f);
		//
		// set the matrix for camera orbiting the origin
		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				3.5,
				Degrees(time * 15),
				Degrees(SineWave(time / 6.3) * 45)
			)
		);
		model_matrix.Set(
			ModelMatrixf::Translation(-1.0, 0.0, 0.0) *
			ModelMatrixf::RotationZ(Degrees(time * 180))
		);
		cube_instr.Draw(cube_indices);
		model_matrix.Set(
			ModelMatrixf::Translation(+1.0, 0.0, 0.0) *
			ModelMatrixf::RotationY(Degrees(time * 90))
		);
		cube_instr.Draw(cube_indices);
	}

	bool Continue(double time)
	{
		return time < 30.0;
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
