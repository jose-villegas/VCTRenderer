/**
 *  @example oglplus/008_lyapunov.cpp
 *  @brief Shows how to draw the Lyapunov fractal
 *
 *  @oglplus_screenshot{008_lyapunov}
 *  @author Timo Keller
 *  @author Matus Chochlik
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

	// VBOs for the rectangle's vertices and the viewport coordinates
	Buffer verts;
	Buffer coords;

	// Scroll factor
	Lazy<Uniform<GLfloat>> scroll_factor;
public:
	RectangleExample(void)
	 : scroll_factor(prog, "ScrollFactor")
	{
		// Vertex shader
		VertexShader vs;
		// Set the vertex shader source
		vs.Source(" \
			#version 120\n \
			uniform float ScrollFactor; \
			attribute vec2 Position; \
			attribute vec2 Coord; \
			varying vec2 vertCoord; \
			void main(void) \
			{ \
				vertCoord = ScrollFactor * Coord; \
				gl_Position = vec4(Position, 0.0, 1.0); \
			} \
		");
		// compile it
		vs.Compile();

		// Fragment shader
		FragmentShader fs;
		// set the fragment shader source
		fs.Source(" \
			#version 120\n \
			varying vec2 vertCoord; \
			const int npasses = 100; \
			const int nclr = 5; \
			uniform vec4 clrs[5]; \
			void main(void) \
			{ \
				vec2 ab = vertCoord; \
				float x_n = 0.5; \
				float sum = 0; \
				for (int i = 0; i < npasses; ++i) \
				{ \
					x_n = ab.x * x_n * (1 - x_n); \
					sum += log(abs(ab.x*(1 - 2 * x_n))); \
					x_n = ab.x * x_n * (1 - x_n); \
					sum += log(abs(ab.x*(1 - 2 * x_n))); \
					x_n = ab.x * x_n * (1 - x_n); \
					sum += log(abs(ab.x*(1 - 2 * x_n))); \
					x_n = ab.x * x_n * (1 - x_n); \
					sum += log(abs(ab.x*(1 - 2 * x_n))); \
					x_n = ab.x * x_n * (1 - x_n); \
					sum += log(abs(ab.x*(1 - 2 * x_n))); \
					x_n = ab.x * x_n * (1 - x_n); \
					sum += log(abs(ab.x*(1 - 2 * x_n))); \
					x_n = ab.y * x_n * (1.0 - x_n); \
					sum += log(abs(ab.y*(1 - 2 * x_n))); \
					x_n = ab.y * x_n * (1.0 - x_n); \
					sum += log(abs(ab.y*(1 - 2 * x_n))); \
					x_n = ab.y * x_n * (1.0 - x_n); \
					sum += log(abs(ab.y*(1 - 2 * x_n))); \
					x_n = ab.y * x_n * (1.0 - x_n); \
					sum += log(abs(ab.y*(1 - 2 * x_n))); \
					x_n = ab.y * x_n * (1.0 - x_n); \
					sum += log(abs(ab.y*(1 - 2 * x_n))); \
					x_n = ab.y * x_n * (1.0 - x_n); \
					sum += log(abs(ab.y*(1 - 2 * x_n))); \
				} \
				float Lambda = sum / (12 * npasses); \
				float a = (clamp(Lambda, -2.0, 2.0) + 2.0) / 4; \
				for(int i = 0; i != (nclr - 1); ++i) \
				{ \
					if(a >= clrs[i].a && a < clrs[i+1].a) \
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
		");
		// compile it
		fs.Compile();

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
		Buffer::Data(Buffer::Target::Array, 8, rectangle_verts);
		// setup the vertex attribs array for the vertices
		VertexArrayAttrib vert_attr(prog, "Position");
		vert_attr.Setup<GLfloat>(2);
		vert_attr.Enable();

		GLfloat min = 3.0f;
		GLfloat max = 4.0f;
		GLfloat rectangle_coords[8] = {
			 min, min,
			 min, max,
			 max, min,
			 max, max
		};
		// bind the VBO for the rectangle vertices
		coords.Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(Buffer::Target::Array, 8, rectangle_coords);
		// setup the vertex attribs array for the vertices
		VertexArrayAttrib coord_attr(prog, "Coord");
		coord_attr.Setup<GLfloat>(2);
		coord_attr.Enable();
		//
		// color map used in the fragment shader to colorize the fractal
		const GLuint nclr = 5;
		GLfloat colormap[nclr*4] = {
			0.4f, 0.2f, 1.0f, 0.00f,
			1.0f, 0.2f, 0.2f, 0.30f,
			1.0f, 1.0f, 1.0f, 0.95f,
			1.0f, 1.0f, 1.0f, 0.98f,
			0.1f, 0.1f, 0.1f, 1.00f
		};
		Uniform<Vec4f>(prog, "clrs").SetValues(nclr*4, colormap);
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

		scroll_factor.Set(GLfloat(1.0 / (0.005 * time + 1.0)));

		gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);
	}

	bool Continue(double time)
	{
		return time < 15.0;
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
