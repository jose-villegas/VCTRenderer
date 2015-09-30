/**
 *  @example oglplus/006_cartoon_sun.cpp
 *  @brief Shows how to render a rectangle with an animated sun drawing
 *
 *  @oglplus_screenshot{006_cartoon_sun}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include "example.hpp"

#include <cmath>

namespace oglplus {

class RectangleExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	// Vertex shader
	VertexShader vs;

	// Fragment shader
	FragmentShader fs;

	// Program
	Program prog;

	// A vertex array object for the rendered rectangle
	VertexArray rectangle;

	// VBOs for the rectangle's vertices used also as the viewport coords
	Buffer verts;
public:
	RectangleExample(void)
	{
		// Set the vertex shader source and compile it
		vs.Source(
			"#version 120\n"
			"attribute vec2 Position;"
			"varying vec2 vertPos;"
			"void main(void)"
			"{"
			"	gl_Position = vec4(Position, 0.0, 1.0);"
			"	vertPos = gl_Position.xy;"
			"}"
		).Compile();

		// set the fragment shader source and compile it
		fs.Source(
			"#version 120\n"
			"#extension GL_EXT_gpu_shader4 : enable\n"
			"uniform float Time;"
			"uniform vec2 SunPos;"
			"uniform vec3 Sun1, Sun2, Sky1, Sky2;"
			"varying vec2 vertPos;"
			"void main(void)"
			"{"
			"	vec2 v = vertPos - SunPos;"
			"	float l = length(v);"
			"	float a = (sin(l)+atan(v.y, v.x))/3.1415;"
			"	if(l < 0.1)"
			"	{"
			"		gl_FragColor = vec4(Sun1, 1);"
			"	}"
			"	else if(int(18*(Time*0.1 + 1.0 + a)) % 2 == 0)"
			"	{"
			"		gl_FragColor = vec4(mix(Sun1, Sun2, l), 1);"
			"	}"
			"	else"
			"	{"
			"		gl_FragColor = vec4(mix(Sky1, Sky2, l), 1);"
			"	}"
			"}"
		).Compile();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the rectangle
		rectangle.Bind();

		GLfloat rectangle_verts[8] = {
			-1.0f, -1.0f,
			-1.0f,  1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f
		};
		// bind the VBO for the rectangle vertices
		verts.Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(Buffer::Target::Array, rectangle_verts);
		// setup the vertex attribs array for the vertices
		(prog|"Position").Setup<Vec2f>().Enable();
		//
		// Uniforms can be typechecked
		// but note that the typechecking is a relatively
		// expensive operation
		Typechecked<Uniform<Vec3f>>(prog, "Sun1").Set(0.95f, 0.85f, 0.60f);
		Typechecked<Uniform<Vec3f>>(prog, "Sun2").Set(0.90f, 0.80f, 0.20f);
		Typechecked<Uniform<Vec3f>>(prog, "Sky1").Set(0.90f, 0.80f, 0.50f);
		Typechecked<Uniform<Vec3f>>(prog, "Sky2").Set(0.80f, 0.60f, 0.40f);
		//
		gl.Disable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer();

		auto angle = FullCircles(GLfloat(time * 0.05f));
		Uniform<GLfloat>(prog, "Time").Set(GLfloat(time));
		Uniform<Vec2f>(prog, "SunPos").Set(
			-Cos(angle),
			Sin(angle)
		);
		gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);
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
	return std::unique_ptr<Example>(new RectangleExample);
}

} // namespace oglplus
