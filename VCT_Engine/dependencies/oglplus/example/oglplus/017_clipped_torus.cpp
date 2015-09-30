/**
 *  @example oglplus/017_clipped_torus.cpp
 *  @brief Shows how to do custom clipping
 *
 *  @oglplus_screenshot{017_clipped_torus}
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
			"#version 130\n"
			"uniform mat4 ProjectionMatrix, ModelMatrix, CameraMatrix;"
			"uniform vec4 ClipPlane;"
			"attribute vec4 Position;"
			"attribute vec2 TexCoord;"
			"varying vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = "
			"		ModelMatrix *"
			"		Position;"
			"	gl_ClipDistance[0] = dot(ClipPlane, gl_Position);"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		gl_Position;"
			"}"
		);
		vs.Compile();

		FragmentShader fs;
		fs.Source(
			"#version 130\n"
			"varying vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	float i = ("
			"		int(vertTexCoord.x*36) % 2+"
			"		int(vertTexCoord.y*24) % 2"
			"	) % 2;"
			"	if(gl_FrontFacing)"
			"	{"
			"		gl_FragColor = vec4(1-i/2, 1-i/2, 1-i/2, 1.0);"
			"	}"
			"	else"
			"	{"
			"		gl_FragColor = vec4(0+i/2, 0+i/2, 0+i/2, 1.0);"
			"	}"
			"}"
		);
		fs.Compile();

		Program prog;
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		prog.Link();
		prog.Use();

		return prog;
	}

	// Uniforms
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	// A vertex array object for the rendered torus
	VertexArray torus;

	// VBOs for the torus's vertices and tex-coordinates
	Buffer verts;
	Buffer texcoords;
public:
	TorusExample(void)
	 : make_torus(1.0, 0.5, 36, 24)
	 , torus_instr(make_torus.Instructions())
	 , torus_indices(make_torus.Indices())
	 , prog(make_prog())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	{
		Uniform<Vec4f>(prog, "ClipPlane").Set(0.f, 0.f, 1.f, 0.f);

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
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// bind the VBO for the torus UV-coordinates
		texcoords.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.TexCoordinates(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "TexCoord");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}
		//
		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.FrontFace(make_torus.FaceWinding());
		gl.Enable(Capability::DepthTest);
		gl.Enable(Functionality::ClipDistance, 0);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				width, height,
				1, 60
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
				4.5f,
				FullCircles(time / 10.0),
				Degrees(45.0 + SineWave(time / 7.0)*30.0)
			)
		);
		model_matrix.Set(ModelMatrixf::RotationX(FullCircles(time / 12.0)));

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
