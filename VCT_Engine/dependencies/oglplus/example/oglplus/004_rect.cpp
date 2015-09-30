/**
 *  @example oglplus/004_rect.cpp
 *  @brief Shows how to draw a custom shaded rectangle
 *
 *  @oglplus_screenshot{004_rect}
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

	// Vertex and fragment shaders
	Shader vs, fs;

	// Program
	Program prog;

	// A vertex array object for the rendered rectangle
	VertexArray rectangle;

	// VBO for the rectangle's vertex positions
	Buffer positions, indices;
public:
	RectangleExample(void)
	 : vs(ShaderType::Vertex)
	 , fs(ShaderType::Fragment)
	{
		// Try to set and compile the vertex shader source
		// if that succeeded continue, otherwise compile
		// the other shader source and require success
		vs.Source(" \
			#version 330\n \
			in vec2 Position; \
			out vec2 vertCoord; \
			void main(void) \
			{ \
				vertCoord = Position; \
				gl_Position = vec4(Position, 0.0, 1.0); \
			} \
		").Compile(std::nothrow).DoneWithoutError() || vs.Source(" \
			#version 120\n \
			attribute vec2 Position; \
			varying vec2 vertCoord; \
			void main(void) \
			{ \
				vertCoord = Position; \
				gl_Position = vec4(Position, 0.0, 1.0); \
			} \
		").Compile(std::nothrow).Done();

		fs.Source(" \
			#version 330\n \
			const float radius = 0.4; \
			in vec2 vertCoord; \
			uniform vec2 RedCenter, GreenCenter, BlueCenter; \
			out vec4 fragColor; \
			void main(void) \
			{ \
				vec3 dist = vec3( \
					distance(vertCoord, RedCenter), \
					distance(vertCoord, GreenCenter), \
					distance(vertCoord, BlueCenter) \
				); \
				fragColor = vec4( \
					dist.r < radius ? 1.0 : (2*radius - dist.r) / radius, \
					dist.g < radius ? 1.0 : (2*radius - dist.g) / radius, \
					dist.b < radius ? 1.0 : (2*radius - dist.b) / radius, \
					1.0 \
				); \
			} \
		").Compile(std::nothrow).DoneWithoutError() || fs.Source("\
			#version 120\n \
			const float radius = 0.4; \
			varying vec2 vertCoord; \
			uniform vec2 RedCenter, GreenCenter, BlueCenter; \
			void main(void) \
			{ \
				vec3 dist = vec3( \
					distance(vertCoord, RedCenter), \
					distance(vertCoord, GreenCenter), \
					distance(vertCoord, BlueCenter) \
				); \
				gl_FragColor = vec4( \
					dist.g < radius ? 1.0 : (2*radius - dist.g) / radius, \
					dist.b < radius ? 1.0 : (2*radius - dist.b) / radius, \
					dist.r < radius ? 1.0 : (2*radius - dist.r) / radius, \
					1.0 \
				); \
			} \
		").Compile(std::nothrow).Done();

		// attach the shaders to the program
		prog.AttachShaders(MakeGroup(vs, fs));
		// link and use it
		prog.Link();
		prog.Use();

		// bind the VAO for the rectangle
		rectangle.Bind();

		GLfloat rectangle_positions[8] = {
			-1.0f, -1.0f,
			-1.0f,  1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f
		};
		// bind the VBO for the rectangle vertices
		positions.Bind(Buffer::Target::Array);
		// upload the data
		Buffer::Data(Buffer::Target::Array, 8, rectangle_positions);
		// setup the vertex attribs array for the vertices
		VertexArrayAttrib vert_attr(prog, "Position");
		vert_attr.Setup<GLfloat>(2);
		vert_attr.Enable();

		GLuint rectangle_indices[4] = {
			0, 1, 2, 3
		};
		indices.Bind(Buffer::Target::ElementArray);
		Buffer::Data(Buffer::Target::ElementArray, 4, rectangle_indices);

		//
		// Variables referencing the program's uniforms
		Uniform<Vec2f>(prog,   "RedCenter").Set(Vec2f(-0.141f, 0.141f));
		Uniform<Vec2f>(prog, "GreenCenter").Set(Vec2f(0.141f, 0.141f));
		Uniform<Vec2f>(prog,  "BlueCenter").Set(Vec2f(0.0f, -0.2f));

		gl.Disable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
	}

	void Render(double)
	{
		gl.Clear().ColorBuffer();

		gl.DrawElements(
			PrimitiveType::TriangleStrip,
			4,
			DataType::UnsignedInt
		);
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
