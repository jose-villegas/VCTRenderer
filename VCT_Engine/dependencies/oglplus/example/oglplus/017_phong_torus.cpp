/**
 *  @example oglplus/017_phong_torus.cpp
 *  @brief Shows how to draw a "phong"-like shaded torus
 *
 *  @oglplus_screenshot{017_phong_torus}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_1}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/twisted_torus.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class TorusExample : public Example
{
private:
	// helper object building torus vertex attributes
	shapes::TwistedTorus make_torus;
	// helper object encapsulating torus drawing instructions
	shapes::DrawingInstructions torus_instr;
	// indices pointing to torus primitive elements
	shapes::TwistedTorus::IndexArray torus_indices;

	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader
	VertexShader vs;

	// Fragment shader
	FragmentShader fs;

	// Program
	Program prog;

	// A vertex array object for the rendered torus
	VertexArray torus;

	// VBOs for the torus's vertex positions, normals and colors
	Buffer positions, normals, colors;
public:
	TorusExample(void)
	 : make_torus()
	 , torus_instr(make_torus.Instructions())
	 , torus_indices(make_torus.Indices())
	{
		// Set the vertex shader source
		vs.Source(
			"#version 150\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec3 Color;"
			"out vec3 vertColor;"
			"out vec3 vertNormal;"
			"out vec3 vertViewDir;"
			"void main(void)"
			"{"
			"	vertColor = normalize(vec3(1,1,1)-Color);"
			"	vertNormal = Normal;"
			"	vertViewDir = ("
			"		vec4(0.0, 0.0, 1.0, 1.0)*"
			"		CameraMatrix"
			"	).xyz;"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		Position;"
			"}"
		);
		// compile it
		vs.Compile();

		// set the fragment shader source
		fs.Source(
			"#version 150\n"
			"in vec3 vertColor;"
			"in vec3 vertNormal;"
			"in vec3 vertViewDir;"
			"out vec4 fragColor;"
			"uniform vec3 LightPos[3];"
			"void main(void)"
			"{"
			"	float amb = 0.2;"
			"	float diff = 0.0;"
			"	float spec = 0.0;"
			"	for(int i=0;i!=3;++i)"
			"	{"
			"		diff += max("
			"			dot(vertNormal,  LightPos[i])/"
			"			dot(LightPos[i], LightPos[i]),"
			"			0.0"
			"		);"
			"		float k = dot(vertNormal, LightPos[i]);"
			"		vec3 r = 2.0*k*vertNormal - LightPos[i];"
			"		spec += pow(max("
			"			dot(normalize(r), vertViewDir),"
			"			0.0"
			"		), 32.0 * dot(r, r));"
			"	}"
			"	fragColor = "
			"		vec4(vertColor, 1.0)*(amb+diff)+"
			"		vec4(1.0, 1.0, 1.0, 1.0)*spec;"
			"}"
		);
		// compile it
		fs.Compile();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the torus
		torus.Bind();

		// bind the VBO for the torus vertex positions
		positions.Bind(Buffer::Target::Array);
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

		// bind the VBO for the torus normals
		normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Normals(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// bind the VBO for the torus colors
		colors.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Tangents(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			VertexArrayAttrib attr(prog, "Color");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// set the light positions
		Uniform<Vec3f> light_pos(prog, "LightPos");
		light_pos[0].Set(Vec3f(2.0f,-1.0f, 0.0f));
		light_pos[1].Set(Vec3f(0.0f, 3.0f,-1.0f));
		light_pos[2].Set(Vec3f(0.0f,-1.0f, 4.0f));
		//
		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.FrontFace(make_torus.FaceWinding());
		gl.CullFace(Face::Back);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		prog.Use();
		Uniform<Mat4f>(prog, "ProjectionMatrix").Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
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
		Uniform<Mat4f>(prog, "CameraMatrix").Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				5.0,
				FullCircles(time / 17.0),
				Degrees(SineWave(time / 19.0) * 90)
			)
		);

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
