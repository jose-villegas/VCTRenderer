/**
 *  @example oglplus/014_multi_cube_ub.cpp
 *  @brief Shows how to transform and draw multiple instances of a cube
 *
 *  @oglplus_screenshot{014_multi_cube_ub}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>

#include <algorithm>

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

	// Program
	Program prog;

	// Uniforms
	Uniform<Mat4f> projection_matrix, camera_matrix;

	// A vertex array object for the rendered cube
	VertexArray cube;

	// VBOs for the cube's vertices and the uniform block
	Buffer verts, block_buf;
public:
	CubeExample(void)
	 : cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	 , projection_matrix(prog)
	 , camera_matrix(prog)
	{
		// Set the vertex shader source and compile it
		VertexShader vs;
		vs.Source(
			"#version 150\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"layout (std140) uniform ModelBlock {"
			"	mat4 ModelMatrices[36];"
			"};"
			"in vec4 Position;"
			"out vec3 vertColor;"
			"void main(void)"
			"{"
			"	mat4 ModelMatrix = ModelMatrices[gl_InstanceID];"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		ModelMatrix *"
			"		Position;"
			"	vertColor = abs(normalize((ModelMatrix*Position).yxz));"
			"}"
		).Compile();

		// set the fragment shader source and compile it
		FragmentShader fs;
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
		prog.AttachShader(vs);
		prog.AttachShader(fs);
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
			(prog|"Position").Setup<GLfloat>(n_per_vertex).Enable();
		}

		// make the matrices
		{
			// 36 x 4x4 matrices
			std::vector<GLfloat> matrix_data(36*16);
			auto p = matrix_data.begin(), e = matrix_data.end();

			Angle<GLfloat> angle, astep = Angle<GLfloat>::Degrees(10);
			while(p != e)
			{
				GLfloat cx = Cos(angle);
				GLfloat sx = Sin(angle);
				auto matrix = Transposed(Mat4f(
					Vec4f( cx, 0.0, -sx, 0.0),
					Vec4f(0.0, 1.0, 0.0, 0.0),
					Vec4f( sx, 0.0,  cx, 0.0),
					Vec4f(0.0, 0.0, 0.0, 1.0)
				) * Mat4f(
					Vec4f(1.0, 0.0, 0.0,12.0),
					Vec4f(0.0, 1.0, 0.0, 0.0),
					Vec4f(0.0, 0.0, 1.0, 0.0),
					Vec4f(0.0, 0.0, 0.0, 1.0)
				));
				p = std::copy(
					Data(matrix),
					Data(matrix)+Size(matrix),
					p
				);
				angle += astep;
			}

			UniformBlock model_block(prog, "ModelBlock");
			model_block.Binding(0);

			block_buf.Bind(Buffer::Target::Uniform);
			Buffer::Data(
				Buffer::Target::Uniform,
				matrix_data,
				BufferUsage::DynamicDraw
			);
			block_buf.BindBaseUniform(0);
		}

		//
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

		// draw 36 instances of the cube
		// the vertex shader will take care of their placement
		cube_instr.Draw(cube_indices, 36);
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
