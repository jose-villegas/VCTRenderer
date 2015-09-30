/**
 *  @example standalone/007_glm_boxes.cpp
 *  @brief Shows interoperability with GLM matrix and vector classes
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include "glut_glew_example.hpp"

#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>

#define GLM_FORCE_RADIANS
#include <oglplus/interop/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class GLMBoxesExample
 : public oglplus::StandaloneExample
{
private:
	oglplus::shapes::Cube make_cube;
	oglplus::shapes::DrawingInstructions cube_instr;
	oglplus::shapes::Cube::IndexArray cube_indices;

	oglplus::Context gl;

	oglplus::VertexShader vs;
	oglplus::FragmentShader fs;
	oglplus::Program prog;

	oglplus::Lazy<oglplus::Uniform<float>> frame_time;

	oglplus::VertexArray cube;
	oglplus::Buffer positions, normals;
public:
	GLMBoxesExample(int, const char**)
	 : cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	 , frame_time(prog, "Time")
	{
		// Set the vertex shader source
		vs.Source(
			"#version 140\n"
			"uniform mat4 CameraMatrix, ScaleMatrix;"
			"uniform vec3 LightPos;"
			"uniform float Time;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"out vec3 vertColor;"
			"void main(void)"
			"{"
			"	float angle = gl_InstanceID * 10 * 2 * 3.14159 / 360.0;"
			"	float ct = cos(angle+Time);"
			"	float st = sin(angle+Time);"
			"	mat4 ModelMatrix = mat4("
			"		 ct, 0.0,  st, 0.0,"
			"		0.0, 1.0, 0.0, 0.0,"
			"		-st, 0.0,  ct, 0.0,"
			"		0.0, 0.0, 0.0, 1.0 "
			"	) * mat4("
			"		 1.0, 0.0, 0.0, 0.0,"
			"		 0.0, 1.0, 0.0, 0.0,"
			"		 0.0, 0.0, 1.0, 0.0,"
			"		12.0, 0.0, 0.0, 1.0 "
			"	) * mat4("
			"		 ct, -st, 0.0, 0.0,"
			"		 st,  ct, 0.0, 0.0,"
			"		0.0, 0.0, 1.0, 0.0,"
			"		0.0, 0.0, 0.0, 1.0 "
			"	);"
			"	gl_Position = "
			"		ModelMatrix *"
			"		ScaleMatrix *"
			"		Position;"

			"	vec3 vertLightDir = normalize(LightPos - gl_Position.xyz);"
			"	vec3 vertNormal = normalize(("
			"		ModelMatrix *"
			"		vec4(Normal, 0.0)"
			"	).xyz);"

			"	gl_Position = CameraMatrix * gl_Position;"

			"	vertColor = abs(normalize("
			"		Normal -"
			"		vec3(1.0, 1.0, 1.0) +"
			"		Position.xyz*0.2"
			"	)) * (0.6 + 0.5*max(dot(vertNormal, vertLightDir), 0.0));"
			"}"
		);
		// compile it
		vs.Compile();

		// set the fragment shader source
		fs.Source(
			"#version 140\n"
			"in vec3 vertColor;"
			"out vec3 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vertColor;"
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

		// bind the VAO for the cube
		cube.Bind();

		// bind the VBO for the cube vertex positions
		positions.Bind(oglplus::Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Positions(data);
			// upload the data
			oglplus::Buffer::Data(oglplus::Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			oglplus::VertexArrayAttrib attr(prog, "Position");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		// bind the VBO for the cube normals
		normals.Bind(oglplus::Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Normals(data);
			// upload the data
			oglplus::Buffer::Data(oglplus::Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			oglplus::VertexArrayAttrib attr(prog, "Normal");
			attr.Setup<GLfloat>(n_per_vertex);
			attr.Enable();
		}

		//
		gl.ClearColor(0.8f, 0.8f, 0.8f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(oglplus::Capability::DepthTest);

		oglplus::Typechecked<oglplus::Uniform<glm::vec3>>(prog, "LightPos").Set(
			glm::vec3(7.0, 3.0, -1.0)
		);

		oglplus::Typechecked<oglplus::Uniform<glm::mat4x4>>(prog, "ScaleMatrix").Set(
			glm::scale(glm::mat4(1.0), glm::vec3(1.0, 0.3, 1.7))
		);
	}

	void Reshape(void)
	{
		gl.Viewport(Width(), Height());

		auto camera = glm::perspective(
			53.0f*3.1415f/180.f,
			GLfloat(Aspect()),
			1.0f, 100.0f
		) * glm::lookAt(
			glm::vec3(21.0f, 7.0f, 0.0f),
			glm::vec3( 0.0f, 0.0f, 0.0f),
			glm::vec3( 0.0f, 1.0f, 0.0f)
		);
		oglplus::Uniform<glm::mat4x4>(prog, "CameraMatrix").Set(camera);
	}

	void Render(void)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		frame_time.Set(FrameTime());

		// draw 36 instances of the cube
		// the vertex shader will take care of their placement
		cube_instr.Draw(cube_indices, 36);
	}
};

int main(int argc, char* argv[])
{
	return oglplus::GlutGlewMain<GLMBoxesExample>(
		"Example usage with GLM matrix and vector classes",
		argc, argv
	);
}

