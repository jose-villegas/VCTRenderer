/**
 *  @example oglplus/007_cubic_bezier.cpp
 *  @brief Shows how to render line approximating a cubic bezier curve
 *
 *  @oglplus_screenshot{007_cubic_bezier}
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
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

	// VAO and VBO for the curve approximation points
	VertexArray curve;
	Buffer curve_verts;
	// The count of curve points
	std::size_t curve_n;

	// VAO and VBO for the curve control points
	VertexArray control;
	Buffer ctrl_verts;

	// Handle for the Color uniform in fragment shader
	Lazy<Uniform<Vec3f>> color;
	// The count of control points
	std::size_t ctrl_n;
public:
	RectangleExample(void)
	 : color(prog, "Color")
	{
		// Set the vertex shader source and compile it
		vs.Source(
			"#version 120\n"
			"attribute vec4 Position;"
			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"}"
		);

		// set the fragment shader source and compile it
		fs.Source(
			"#version 120\n"
			"uniform vec3 Color;"
			"void main(void)"
			"{"
			"	gl_FragColor = vec4(Color, 1.0);"
			"}"
		);

		// attach the shaders to the program
		// link and use it
		prog.AttachShader(vs).AttachShader(fs).Build().Then().Use();

		// the series of cubic bezier curves
		Vec2f bezier_cps[] = {
			Vec2f(-0.9f, -0.9f),
			Vec2f(-0.9f, +0.9f),
			Vec2f(+0.9f, +0.9f),
			Vec2f(+0.9f,  0.0f),
			Vec2f(+0.9f, -0.9f),
			Vec2f( 0.0f, -0.9f),
			Vec2f( 0.0f, +0.5f),
			Vec2f(+0.5f, +0.5f),
			Vec2f(+0.7f, +0.5f),
			Vec2f(+0.7f, +0.0f),
			Vec2f(-0.7f, -0.2f),
			Vec2f(-0.8f, +0.8f),
			Vec2f(+0.9f, +0.9f),
		};
		BezierCurves<Vec2f, double, 3> bezier(
			std::vector<Vec2f>(
				bezier_cps,
				bezier_cps+sizeof(bezier_cps)/sizeof(bezier_cps[0])
			)
		);

		gl.Bind(curve);
		{
			auto data = bezier.Approximate(25);
			curve_n = data.size();
			gl.Bound(Buffer::Target::Array, curve_verts).Data(data);
			(prog|"Position").Setup<Vec2f>().Enable();
		}

		gl.Bind(control);
		{
			auto data = bezier.ControlPoints();
			ctrl_n = data.size();
			gl.Bound(Buffer::Target::Array, ctrl_verts).Data(data);
			(prog|"Position").Setup<Vec2f>().Enable();
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
		// draw the lines between control points
		color.Set(Vec3f(0.9f, 0.9f, 0.2f));
		gl.Bind(control);
		gl.DrawArrays(PrimitiveType::LineStrip, 0, ctrl_n);
		// draw the curve
		color.Set(Vec3f(0.1f, 0.1f, 0.1f));
		gl.Bind(curve);
		gl.DrawArrays(PrimitiveType::LineStrip, 0, curve_n);
		// draw the control points
		color.Set(Vec3f(0.9f, 0.0f, 0.0f));
		gl.Bind(control);
		gl.PointSize(8.0);
		gl.DrawArrays(PrimitiveType::Points, 0, ctrl_n);
		gl.PointSize(1.0);
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
