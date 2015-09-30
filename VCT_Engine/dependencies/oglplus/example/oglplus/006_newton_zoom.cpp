/**
 *  @example oglplus/006_newton_zoom.cpp
 *  @brief Shows how to do a zooming animation of the Newton fractal.
 *
 *  @oglplus_screenshot{006_newton_zoom}
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

	// VBOs for the rectangle's vertices used also as the viewport coords
	Buffer verts;

	// Uniform referencing the "ZoomMatrix" variable of Program
	Uniform<Mat2f> zoom_matrix;
public:
	RectangleExample(void)
	 : zoom_matrix(prog)
	{
		// Vertex shader
		VertexShader vs;
		// Set the vertex shader source and compile it
		vs.Source(" \
			#version 120\n \
			uniform mat2 ZoomMatrix; \
			attribute vec2 Position; \
			varying vec2 vertCoord; \
			void main(void) \
			{ \
				vertCoord = ZoomMatrix * Position; \
				gl_Position = vec4(Position, 0.0, 1.0); \
			} \
		").Compile();

		// Fragment shader
		FragmentShader fs;
		// set the fragment shader source and compile it
		fs.Source(" \
			#version 120\n \
			varying vec2 vertCoord; \
			uniform vec3 Color1, Color2; \
			\
			vec2 f(vec2 n) \
			{ \
				return vec2( \
					n.x*n.x*n.x - 3.0*n.x*n.y*n.y - 1.0, \
					-n.y*n.y*n.y + 3.0*n.x*n.x*n.y \
				); \
			} \
			\
			vec2 df(vec2 n) \
			{ \
				return 3.0 * vec2( \
					n.x*n.x - n.y*n.y, \
					2.0 * n.x * n.y \
				); \
			} \
			\
			vec2 cdiv(vec2 a, vec2 b) \
			{ \
				float d = dot(b, b); \
				if(d == 0.0) return a; \
				else return vec2( \
					(a.x*b.x + a.y*b.y) / d, \
					(a.y*b.x - a.x*b.y) / d  \
				); \
			} \
			void main(void) \
			{ \
				vec2 z = vertCoord; \
				int i, max = 128; \
				for(i = 0; i != max; ++i) \
				{ \
					vec2 zn = z - cdiv(f(z), df(z)); \
					if(distance(zn, z) < 0.00001) break; \
					z = zn; \
				} \
				gl_FragColor = vec4( \
					mix( \
						Color1.rgb, \
						Color2.rgb, \
						float(i) / float(max)\
					), \
					1.0 \
				); \
			} \
		").Compile();

		// attach the shaders to the program, link and use it
		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		zoom_matrix.BindTo("ZoomMatrix");

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
		(prog/"Color1").Set(Vec3f(0.2f, 0.02f, 0.05f));
		(prog/"Color2").Set(Vec3f(1.0f, 0.95f, 0.98f));
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

		GLfloat scale = GLfloat(1.0 / (3.0 * time + 1.0));
		Angle<GLfloat> angle = FullCircles(time * 0.1);
		Vec2f x(Cos(angle), Sin(angle));
		Vec2f y = Perpendicular(x);

		zoom_matrix = Mat2f(x*scale,  y* scale);

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
