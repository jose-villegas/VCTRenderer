/**
 *  @example oglplus/015_obj_mesh.cpp
 *  @brief Shows how to use the obj file mesh loader
 *
 *  @oglplus_screenshot{015_obj_mesh}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_model{suzanne}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/obj_mesh.hpp>
#include <oglplus/opt/resources.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

struct MeshInputFile
{
	std::ifstream stream;

	MeshInputFile(void)
	{
		OpenResourceFile(stream, "models", "suzanne", ".obj");
	}
};

class MeshExample : public Example
{
private:
	MeshInputFile mesh_input;
	shapes::ObjMesh load_mesh;
	shapes::DrawingInstructions mesh_instr;
	shapes::ObjMesh::IndexArray mesh_indices;

	Context gl;

	Program prog;

	Uniform<Mat4f> camera_matrix, projection_matrix, stretch_matrix;

	VertexArray mesh;
	Spheref mesh_bs;

	Buffer positions;
	Buffer normals;
	Buffer texcoords;
public:
	MeshExample(void)
	 : load_mesh(mesh_input.stream)
	 , mesh_instr(load_mesh.Instructions())
	 , mesh_indices(load_mesh.Indices())
	 , camera_matrix(prog)
	 , projection_matrix(prog)
	 , stretch_matrix(prog)
	{
		load_mesh.BoundingSphere(mesh_bs);

		VertexShader vs;
		vs.Source(
			"#version 120\n"
			"uniform mat4 StretchMatrix, ProjectionMatrix, CameraMatrix;"
			"attribute vec4 Position;"
			"attribute vec3 Normal;"

			"varying vec3 vertNormal;"
			"void main(void)"
			"{"
			"	vertNormal = Normal;"
			"	gl_Position = "
			"		StretchMatrix *"
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		Position;"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 120\n"
			"varying vec3 vertNormal;"
			"void main(void)"
			"{"
			"	vec3 c = normalize(vec3(1, 1, 1) - vertNormal);"
			"	gl_FragColor = vec4(c, 1);"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs);
		prog.Link().Use();

		camera_matrix.BindTo("CameraMatrix");
		projection_matrix.BindTo("ProjectionMatrix");
		stretch_matrix.BindTo("StretchMatrix");

		mesh.Bind();

		positions.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = load_mesh.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);
			(prog|"Position").Setup<GLfloat>(n_per_vertex).Enable();
		}

		normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = load_mesh.Normals(data);
			Buffer::Data(Buffer::Target::Array, data);
			(prog|"Normal").Setup<GLfloat>(n_per_vertex).Enable();
		}

		//
		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(70),
				GLfloat(width)/height,
				1, 20
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		unsigned x=0, y=0, nx=1, ny=1;
		switch(int(time / 3.0) % 5)
		{
			case 1: x=1; y=1; nx=4; ny=4; break;
			case 2: x=2; y=1; nx=4; ny=4; break;
			case 3: x=1; y=2; nx=4; ny=4; break;
			case 4: x=2; y=2; nx=4; ny=4; break;
			default:;
		}
		stretch_matrix.Set(CamMatrixf::ScreenTile(x, y, nx, ny));

		camera_matrix.Set(
			CamMatrixf::Orbiting(
				mesh_bs.Center(),
				mesh_bs.Radius()*1.4f+1.0f,
				FullCircles(-time / 13.0),
				Degrees(SineWave(time / 17.0) * 80)
			)
		);

		mesh_instr.Draw(mesh_indices);
	}

	bool Continue(double time)
	{
		return time < 30.0;
	}

	double ScreenshotTime(void) const
	{
		return 2.5;
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
	return std::unique_ptr<Example>(new MeshExample);
}

} // namespace oglplus
