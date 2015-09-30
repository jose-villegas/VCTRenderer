/**
 *  @example oglplus/012_checker_sphere.cpp
 *  @brief Shows how to draw a checkered sphere using the Sphere builder
 *
 *  @oglplus_screenshot{012_checker_sphere}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_1}
 *  @oglplus_example_uses_cpp_feat{SCOPED_ENUMS}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/sphere.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class SphereExample : public Example
{
private:
	// helper object building sphere vertex attributes
	shapes::Sphere make_sphere;
	// helper object encapsulating sphere drawing instructions
	shapes::DrawingInstructions sphere_instr;
	// indices pointing to sphere primitive elements
	shapes::Sphere::IndexArray sphere_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Program
	Program prog;

	// A vertex array object for the rendered sphere
	VertexArray sphere;

	// VBOs for the sphere's vertices and tex-coordinates
	Buffer verts;
	Buffer texcoords;

	// Uniform variables
	Lazy<Uniform<Mat4f>> projection_matrix;
	Lazy<Uniform<Mat4f>> camera_matrix;
public:
	SphereExample(void)
	 : sphere_instr(make_sphere.Instructions())
	 , sphere_indices(make_sphere.Indices())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	{
		// Vertex shader
		VertexShader vs;
		// Set the vertex shader source and compile it
		vs.Source(
			"#if GL_ARB_explicit_attrib_location\n"
			"#version 140\n"
			"#extension GL_ARB_explicit_attrib_location : enable\n"
			"#else\n"
			"#version 330\n"
			"#endif\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"layout(location = 0) in vec4 Position;"
			"layout(location = 1) in vec2 TexCoord;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		Position;"
			"}"
		).Compile();

		// Fragment shader
		FragmentShader fs;
		// set the fragment shader source and compile it
		fs.Source(
			"#version 140\n"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float i = ("
			"		int(vertTexCoord.x*18) % 2+"
			"		int(vertTexCoord.y*14) % 2"
			"	) % 2;"
			"	fragColor = vec4(1-i/2, 1-i/2, 1-i/2, 1.0);"
			"}"
		).Compile();

		// attach the shaders to the program
		prog.AttachShader(vs).AttachShader(fs);
		// link and use it
		prog.Link().Use();

		// bind the VAO for the sphere
		sphere.Bind();

		// bind the VBO for the sphere vertices
		verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_sphere.Positions(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			// (prog|0) is equivalent to VertexArrayAttrib(prog, VertexAtribSlot(0))
			(prog|0).Setup<GLfloat>(n_per_vertex).Enable();
		}

		// bind the VBO for the sphere UV-coordinates
		texcoords.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_sphere.TexCoordinates(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			// (prog|1) is equivalent to VertexArrayAttrib(prog, VertexAtribSlot(1))
			(prog|1).Setup<GLfloat>(n_per_vertex).Enable();
		}
		//
		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.ClearDepth(1.0f);
		Capability::DepthTest << true;
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				float(width)/height,
				1, 20
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
				Degrees(time * 135),
				Degrees(SineWave(time / 20.0) * 90)
			)
		);

		sphere_instr.Draw(sphere_indices);
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
	return std::unique_ptr<Example>(new SphereExample);
}

} // namespace oglplus
