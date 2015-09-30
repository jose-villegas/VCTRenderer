/**
 *  @example oglplus/016_cartoon_torus.cpp
 *  @brief Shows how to draw a cartoon-like cel shaded torus
 *
 *  @oglplus_screenshot{016_cartoon_torus}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_1}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/torus.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class TorusExample : public Example
{
private:
	// helper object building torus vertex attributes
	shapes::Torus make_torus;
	// helper object encapsulating torus drawing instructions
	shapes::DrawingInstructions torus_instr;
	// indices pointing to torus primitive elements
	shapes::Torus::IndexArray torus_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Program
	Program prog;
	static Program make_prog(void)
	{
		VertexShader vs;
		vs.Source(
			"#version 120\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"attribute vec4 Position;"
			"attribute vec3 Normal;"
			"varying vec3 vertNormal;"
			"void main(void)"
			"{"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		ModelMatrix *"
			"		Position;"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 120\n"
			"varying vec3 vertNormal;"
			"uniform vec3 LightPos;"
			"void main(void)"
			"{"
			"	float intensity = 2.0 * max("
			"		dot(vertNormal,  LightPos)/"
			"		length(LightPos),"
			"		0.0"
			"	);"
			"	if(!gl_FrontFacing)"
			"	{"
			"		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);"
			"	}"
			"	else if(intensity > 0.9)"
			"	{"
			"		gl_FragColor = vec4(1.0, 0.9, 0.8, 1.0);"
			"	}"
			"	else if(intensity > 0.1)"
			"	{"
			"		gl_FragColor = vec4(0.7, 0.6, 0.4, 1.0);"
			"	}"
			"	else"
			"	{"
			"		gl_FragColor = vec4(0.3, 0.2, 0.1, 1.0);"
			"	}"
			"}"
		).Compile();

		Program prog;
		prog << vs << fs;
		prog.Link().Use();
		return prog;
	}

	// Uniforms
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	// A vertex array object for the rendered torus
	VertexArray torus;

	// VBOs for the torus's vertices and normals
	Buffer verts, normals;
public:
	TorusExample(void)
	 : make_torus(1.0, 0.5, 72, 48)
	 , torus_instr(make_torus.Instructions())
	 , torus_indices(make_torus.Indices())
	 , prog(make_prog())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	{
		// bind the VAO for the torus
		torus.Bind();

		// bind the VBO for the torus vertices
		verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Positions(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			(prog|"Position").Setup<GLfloat>(n_per_vertex).Enable();
		}

		// bind the VBO for the torus normals
		normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Normals(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			(prog|"Normal").Setup<GLfloat>(n_per_vertex).Enable();
		}
		//
		// set the light position
		(prog/"LightPos").Set(Vec3f(4.0f, 4.0f, -8.0f));

		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.FrontFace(make_torus.FaceWinding());
		gl.CullFace(Face::Back);
		glLineWidth(4.0f);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(75),
				float(width)/height,
				1, 30
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
				3.5,
				Degrees(time * 35),
				Degrees(SineWave(time / 20.0) * 60)
			)
		);
		// set the model matrix
		model_matrix.Set(
			ModelMatrixf::RotationY(FullCircles(time * 0.25)) *
			ModelMatrixf::RotationX(FullCircles(0.25))
		);

		gl.PolygonMode(PolygonMode::Line);
		gl.CullFace(Face::Front);
		torus_instr.Draw(torus_indices);
		//
		gl.PolygonMode(PolygonMode::Fill);
		gl.CullFace(Face::Back);
		torus_instr.Draw(torus_indices);
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
	return std::unique_ptr<Example>(new TorusExample);
}

} // namespace oglplus
