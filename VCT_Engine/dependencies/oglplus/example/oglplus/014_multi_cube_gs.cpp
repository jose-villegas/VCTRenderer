/**
 *  @example oglplus/014_multi_cube_gs.cpp
 *  @brief Shows how to draw multiple instances of a cube using geometry shader
 *
 *  @oglplus_screenshot{014_multi_cube_gs}
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

	// Geometry shader
	GeometryShader gs;

	// Fragment shader
	FragmentShader fs;

	// Program
	Program prog;

	// Handle for uniforms in program
	Uniform<Mat4f> projection_matrix, camera_matrix;

	// A vertex array object for the rendered cube
	VertexArray cube;

	// VBOs for the cube's vertices
	Buffer verts;
public:
	CubeExample(void)
	 : cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	 , projection_matrix(prog)
	 , camera_matrix(prog)
	{
		// Set the vertex shader source and compile it
		vs.Source(
			"#version 150\n"
			"in vec4 Position;"
			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"}"
		).Compile();

		// Set the geometry shader source and compile it
		gs.Source(
			"#version 150\n"
			"layout(triangles) in;"
			"layout(triangle_strip, max_vertices = 108) out;"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"out vec3 vertColor;"
			"void main(void)"
			"{"
			"	for(int c=0; c!=36; ++c)"
			"	{"
			"		float angle = c * 10 * 2 * 3.14159 / 360.0;"
			"		float cx = cos(angle);"
			"		float sx = sin(angle);"
			"		mat4 ModelMatrix = mat4("
			"			 cx, 0.0,  sx, 0.0,"
			"			0.0, 1.0, 0.0, 0.0,"
			"			-sx, 0.0,  cx, 0.0,"
			"			0.0, 0.0, 0.0, 1.0 "
			"		) * mat4("
			"			 1.0, 0.0, 0.0, 0.0,"
			"			 0.0, 1.0, 0.0, 0.0,"
			"			 0.0, 0.0, 1.0, 0.0,"
			"			12.0, 0.0, 0.0, 1.0 "
			"		);"
			"		for(int v=0; v!=gl_in.length(); ++v)"
			"		{"
			"			vec4 vert = gl_in[v].gl_Position;"
			"			gl_Position = "
			"				ProjectionMatrix *"
			"				CameraMatrix *"
			"				ModelMatrix *"
			"				vert;"
			"			vertColor = abs(normalize(ModelMatrix*vert)).xzy;"
			"			EmitVertex();"
			"		}"
			"		EndPrimitive();"
			"	}"
			"}"
		).Compile();

		// set the fragment shader source and compile it
		fs.Source(
			"#version 150\n"
			"in vec3 vertColor;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vec4(vertColor, 1.0);"
			"}"
		).Compile();

		// attach the shaders to the program
		prog << vs << gs << fs;
		// link and use it
		prog.Link().Use();

		projection_matrix.BindTo("ProjectionMatrix");
		camera_matrix.BindTo("CameraMatrix");

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

		gl.ClearColor(0.9f, 0.9f, 0.9f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(70),
				width, height,
				1, 50
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		// set the matrix for camera orbiting the origin
		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				18.5,
				Degrees(time * 135),
				Degrees(SineWave(time / 20.0) * 30)
			)
		);

		// draw 1 instance of the cube
		// the geometry shader will spawn 36 copies
		// and take care of their placement
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
