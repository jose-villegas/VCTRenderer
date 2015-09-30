/**
 *  @example oglplus/023_reflected_cube.cpp
 *  @brief Shows how to draw a cube reflected in a horizontal plane
 *
 *  @oglplus_screenshot{023_reflected_cube}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cage.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class ReflectionExample : public Example
{
private:
	// the cube vertex attribute builder
	shapes::Cage make_cube;
	// here will be stored the indices used by the drawing instructions
	shapes::Cage::IndexArray cube_indices;
	// the instructions for drawing the cube
	shapes::DrawingInstructions cube_instr;

	// wrapper around the current OpenGL context
	Context gl;

	// Programs
	Program prog;
	static Program make_prog(void)
	{
		VertexShader vs;
		vs.Source(
			"#version 140\n"
			"in vec4 Position;"
			"in vec3 Normal;"
			"out vec3 vertColor;"
			"out vec3 vertNormal;"
			"out vec3 vertLight;"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"uniform vec3 LightPos;"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertColor = abs(normalize(Normal+vec3(1, 1, 1)));"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertLight = LightPos - gl_Position.xyz;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);
		vs.Compile();

		FragmentShader fs;
		fs.Source(
			"#version 140\n"
			"in vec3 vertColor;"
			"in vec3 vertNormal;"
			"in vec3 vertLight;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float l = dot(vertLight, vertLight);"
			"	float d = l > 0.0 ? dot(vertNormal, normalize(vertLight)) / l : 0.0;"
			"	float i = 0.2 + max(d*3.2, 0.0);"
			"	fragColor = vec4(vertColor*i, 1.0);"
			"}"
		);
		fs.Compile();

		Program prog;
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		prog.Link();

		return prog;
	}

	// Uniforms
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	// A vertex array object for the cube
	VertexArray cube;
	// A vertex array object for the reflective plane
	VertexArray plane;

	// VBOs for the cube's vertices and normals
	Buffer cube_verts, cube_normals;
	// VBOs for the plane's vertices and normals
	Buffer plane_verts, plane_normals;
public:
	ReflectionExample(void)
	 : make_cube(0.5,0.5,0.5, 0.1,0.1,0.1, 3,3,3)
	 , cube_indices(make_cube.Indices())
	 , cube_instr(make_cube.Instructions())
	 , prog(make_prog())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	{
		gl.Use(prog);

		// bind the VAO for the cube
		gl.Bind(cube);

		// bind the VBO for the cube vertices
		gl.Bind(Buffer::Target::Array, cube_verts);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Positions(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// bind the VBO for the cube normals
		gl.Bind(Buffer::Target::Array, cube_normals);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Normals(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the normals
			VertexArrayAttrib attr(prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// bind the VAO for the plane
		gl.Bind(plane);

		// bind the VBO for the plane vertices
		gl.Bind(Buffer::Target::Array, plane_verts);
		{
			GLfloat data[4*3] = {
				-2.0f, 0.0f,  2.0f,
				-2.0f, 0.0f, -2.0f,
				 2.0f, 0.0f,  2.0f,
				 2.0f, 0.0f, -2.0f
			};
			// upload the data
			Buffer::Data(Buffer::Target::Array, 4*3, data);
			// setup the vertex attribs array for the vertices
			prog.Use();
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<Vec3f>();
			attr.Enable();
		}

		// bind the VBO for the cube normals
		gl.Bind(Buffer::Target::Array, plane_normals);
		{
			GLfloat data[4*3] = {
				-0.1f, 1.0f,  0.1f,
				-0.1f, 1.0f, -0.1f,
				 0.1f, 1.0f,  0.1f,
				 0.1f, 1.0f, -0.1f
			};
			// upload the data
			Buffer::Data(Buffer::Target::Array, 4*3, data);
			// setup the vertex attribs array for the normals
			prog.Use();
			VertexArrayAttrib attr(prog, "Normal");
			attr.Setup<Vec3f>();
			attr.Enable();
		}
		gl.Bind(NoVertexArray());

		Uniform<Vec3f>(prog, "LightPos").Set(1.5f, 2.0f, 2.5f);
		//
		gl.ClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.ClearStencil(0);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				width, height,
				1, 20
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer().StencilBuffer();
		// make the camera matrix orbiting around the origin
		// at radius of 3.5 with elevation between 15 and 90 degrees
		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				5.0,
				Degrees(time * 11),
				Degrees(15 + (-SineWave(0.25+time/12.5)+1.0)*0.5*75)
			)
		);
		ModelMatrixf identity;
		// make the model transformation matrix
		ModelMatrixf model =
			ModelMatrixf::Translation(0.0f, 1.5f, 0.0) *
			ModelMatrixf::RotationZ(Degrees(time * 43))*
			ModelMatrixf::RotationY(Degrees(time * 63))*
			ModelMatrixf::RotationX(Degrees(time * 79));
		// make the reflection matrix
		auto reflection = ModelMatrixf::Reflection(false, true, false);
		//
		gl.Disable(Capability::Blend);
		gl.Disable(Capability::DepthTest);
		gl.Enable(Capability::StencilTest);
		gl.ColorMask(false, false, false, false);
		gl.StencilFunc(CompareFunction::Always, 1, 1);
		gl.StencilOp(StencilOp::Keep, StencilOp::Keep, StencilOp::Replace);

		gl.Bind(plane);
		model_matrix.Set(identity);
		gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);

		gl.ColorMask(true, true, true, true);
		gl.Enable(Capability::DepthTest);
		gl.StencilFunc(CompareFunction::Equal, 1, 1);
		gl.StencilOp(StencilOp::Keep, StencilOp::Keep, StencilOp::Keep);

		// draw the cube using the reflection program
		model_matrix.Set(reflection * model);
		gl.Bind(cube);
		cube_instr.Draw(cube_indices);

		gl.Disable(Capability::StencilTest);

		// draw the cube using the normal object program
		model_matrix.Set(model);
		cube_instr.Draw(cube_indices);

		// blend-in the plane
		gl.Enable(Capability::Blend);
		gl.BlendEquation(BlendEquation::Max);
		gl.Bind(plane);
		model_matrix.Set(identity);
		gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);
	}

	bool Continue(double time)
	{
		return time < 60.0;
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
	return std::unique_ptr<Example>(new ReflectionExample);
}

} // namespace oglplus
