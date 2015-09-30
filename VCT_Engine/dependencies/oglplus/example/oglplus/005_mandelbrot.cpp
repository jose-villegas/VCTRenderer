/**
 *  @example oglplus/005_mandelbrot.cpp
 *  @brief Shows how to draw a rectangle with the mandelbrot set
 *
 *  @oglplus_screenshot{005_mandelbrot}
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include "example.hpp"

namespace oglplus {

class RectangleExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	// Program
	Program prog;

	// A vertex array object for the rendered rectangle
	VertexArray rectangle;

	// VBOs for the rectangle's vertices and the mandelbrot set viewport
	Buffer verts;
	Buffer coords;
public:
	RectangleExample(void)
	{
		// Vertex shader
		VertexShader vs;
		// Set the vertex shader source
		vs.Source(StrCRef(" \
			#version 120\n \
			attribute vec2 Position; \
			attribute vec2 Coord; \
			varying vec2 vertCoord; \
			void main(void) \
			{ \
				vertCoord = Coord; \
				gl_Position = vec4(Position, 0.0, 1.0); \
			} \
		"));

		// Fragment shader
		FragmentShader fs;
		// set the fragment shader source
		fs.Source(StrCRef(" \
			#version 120\n \
			varying vec2 vertCoord; \
			const int nclr = 5; \
			uniform vec4 clrs[5]; \
			void main(void) \
			{ \
				vec2 z = vec2(0.0, 0.0); \
				vec2 c = vertCoord; \
				int i = 0, max = 128; \
				while((i != max) && (distance(z, c) < 2.0)) \
				{ \
					vec2 zn = vec2( \
						z.x * z.x - z.y * z.y + c.x, \
						2.0 * z.x * z.y + c.y \
					); \
					z = zn; \
					++i; \
				} \
				float a = sqrt(float(i) / float(max)); \
				for(i = 0; i != (nclr - 1); ++i) \
				{ \
					if(a > clrs[i].a && a <= clrs[i+1].a) \
					{ \
						float m = (a - clrs[i].a) / (clrs[i+1].a - clrs[i].a); \
						gl_FragColor = vec4( \
							mix(clrs[i].rgb, clrs[i+1].rgb, m), \
							1.0 \
						); \
						break; \
					} \
				} \
			} \
		"));

		// attach the shaders to the program
		prog.AttachShader(vs);
		prog.AttachShader(fs);
		// compile, link and use it
		prog.Build();
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
		// (prog|"Position") is equivalent to VertexArrayAttrib(prog, "Position")
		(prog|"Position").Setup<Vec2f>().Enable();

		GLfloat rectangle_coords[8] = {
			-1.5f, -0.5f,
			-1.5f,  1.0f,
			 0.5f, -0.5f,
			 0.5f,  1.0f
		};
		// bind the VBO for the rectangle vertices
		coords.Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(Buffer::Target::Array, rectangle_coords);
		// setup the vertex attribs array for the vertices
		// (prog|"Coord") is equivalent to VertexArrayAttrib(prog, "Coord")
		(prog|"Coord").Setup<Vec2f>().Enable();
		//
		// color map used in the fragment shader to colorize the fractal
		const std::size_t nclr = 5;
		GLfloat colormap[nclr*4] = {
			0.4f, 0.2f, 1.0f, 0.00f,
			1.0f, 0.2f, 0.2f, 0.30f,
			1.0f, 1.0f, 1.0f, 0.95f,
			1.0f, 1.0f, 1.0f, 0.98f,
			0.1f, 0.1f, 0.1f, 1.00f
		};
		Uniform<Vec4f>(prog, "clrs").SetValues(nclr*4, colormap);
		gl.Disable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
	}

	void Render(double)
	{
		gl.Clear().ColorBuffer();

		gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);
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
