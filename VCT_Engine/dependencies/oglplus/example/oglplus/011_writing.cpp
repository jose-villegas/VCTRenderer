/**
 *  @example oglplus/011_writing.cpp
 *  @brief Shows how to use a bezier spline and geometry shader to write a text
 *
 *  @oglplus_screenshot{011_writing}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/bound/buffer.hpp>

#include "example.hpp"

#include <cmath>

namespace oglplus {

class WritingExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	// Program
	Program prog;

	// VAO and VBO for the curve approximation points
	VertexArray writing;
	Buffer curve_verts;
	// The count of curve points
	std::size_t curve_n;
public:
	WritingExample(void)
	{
		VertexShader vs;
		// Set the vertex shader source
		vs.Source(
			"#version 150\n"
			"in vec4 Position;"
			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"}"
		);
		// compile it
		vs.Compile();

		GeometryShader gs;
		// Set the geometry shader source
		gs.Source(
			"#version 150\n"
			"layout(lines) in;"
			"layout(triangle_strip, max_vertices = 4) out;"
			"void main(void)"
			"{"
			"	vec4 offs = vec4(0.02, 0.01, 0.0, 0.0);"
			"	gl_Position = gl_in[0].gl_Position - offs;"
			"	EmitVertex();"
			"	gl_Position = gl_in[0].gl_Position + offs;"
			"	EmitVertex();"
			"	gl_Position = gl_in[1].gl_Position - offs;"
			"	EmitVertex();"
			"	gl_Position = gl_in[1].gl_Position + offs;"
			"	EmitVertex();"
			"	EndPrimitive();"
			"}"
		);
		// compile it
		gs.Compile();

		FragmentShader fs;
		// set the fragment shader source
		fs.Source(
			"#version 150\n"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vec4(0.0, 0.0, 0.0, 1.0);"
			"}"
		);
		// compile it
		fs.Compile();

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(gs);
		prog.AttachShader(fs);
		// link and use it
		prog.Link();
		gl.Use(prog);

		const Vec2f points[] = {
			Vec2f(-0.33f, +0.50f),
			Vec2f(-0.45f, +0.70f),
			Vec2f(-0.66f, +0.70f),
			Vec2f(-0.66f, +0.30f),
			Vec2f(-0.66f, -0.20f),
			Vec2f(-0.35f, -0.15f),
			Vec2f(-0.30f, +0.05f),
			Vec2f(-0.20f, +0.50f),
			Vec2f(-0.30f, +0.50f),
			Vec2f(-0.33f, +0.50f),
			Vec2f(-0.50f, +0.45f),
			Vec2f(-0.10f, +0.40f),
			Vec2f(+0.10f, +0.55f),
			Vec2f(-0.20f, +0.40f),
			Vec2f(-0.30f, -0.10f),
			Vec2f( 0.00f, -0.10f),
			Vec2f(+0.10f, -0.10f),
			Vec2f(+0.20f, -0.10f),
			Vec2f(+0.10f, +0.55f),
			Vec2f(+0.20f, +0.00f),
			Vec2f(+0.30f, -0.70f),
			Vec2f( 0.00f, -0.75f),
			Vec2f(-0.40f, -0.75f),
			Vec2f( 0.00f,  0.00f),
			Vec2f(+0.40f, +0.10f),
			Vec2f(+0.60f, +0.10f),
			Vec2f(+0.70f, +0.90f),
			Vec2f(+0.55f, +0.90f),
			Vec2f(+0.35f, +0.90f),
			Vec2f(+0.10f, -0.10f),
			Vec2f(+0.55f,  0.00f),
			Vec2f(+0.90f,  0.10f),
			Vec2f(+0.70f,  0.10f),
			Vec2f(+0.90f,  0.20f)
		};
		BezierCurves<Vec2f, double, 3> bezier(
			std::vector<Vec2f>(
				points,
				points+sizeof(points)/sizeof(points[0])
			)
		);

		gl.Bind(writing);
		gl.Bind(Buffer::Target::Array, curve_verts);
		{
			auto data = bezier.Approximate(25);
			curve_n = data.size();
			gl.Current(Buffer::Target::Array).Data(data);
			VertexArrayAttrib attr(prog, "Position");
			attr.Setup<Vec2f>();
			attr.Enable();
		}

		gl.ClearColor(0.9f, 0.9f, 0.9f, 0.0f);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
	}

	void Render(double /*time*/)
	{
		gl.Clear().ColorBuffer();
		// draw the curve
		gl.DrawArrays(PrimitiveType::LineStrip, 0, curve_n);
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
	return std::unique_ptr<Example>(new WritingExample);
}

} // namespace oglplus
