/**
 *  @example oglplus/007_voronoi.cpp
 *  @brief Shows an implementation of Voronoi shader
 *
 *  @oglplus_screenshot{007_voronoi}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/screen.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/images/random.hpp>

#include <oglplus/opt/list_init.hpp>

#include "example.hpp"

#include <cstdlib>
#include <ctime>

namespace oglplus {

class TexProgram
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		Shader vs(ShaderType::Vertex);
		vs.Source(
		"#version 140\n"

		"uniform vec2 Offset;"
		"uniform float Scale;"

		"in vec4 Position;"

		"out vec2 vertTexCoord;"

		"void main(void)"
		"{"
		"	gl_Position = Position;"
		"	vertTexCoord = Scale * (0.5*Position.xy + Offset);"
		"}"
		).Compile();

		Shader fs(ShaderType::Fragment);
		fs.Source(
		"#version 140\n"

		"uniform float Scale;"
		"uniform sampler2D Tex;"

		"in vec2 vertTexCoord;"

		"out vec3 fragColor;"

		"const vec2 offs[9] = vec2[9]("
		"	vec2(-1,-1),"
		"	vec2(-1, 0),"
		"	vec2(-1, 1),"
		"	vec2( 0,-1),"
		"	vec2( 0, 0),"
		"	vec2( 0, 1),"
		"	vec2( 1,-1),"
		"	vec2( 1, 0),"
		"	vec2( 1, 1) "
		");"

		"float dist(vec2 tc, vec2 ofs)"
		"{"
		"	vec2 cc = floor(tc+ofs);"
		"	vec2 cp = texture(Tex, cc/textureSize(Tex, 0)).xy;"
		"	return distance(tc, cc+cp);"
		"}"

		"vec3 point_color(vec2 tc, vec2 ofs)"
		"{"
		"	vec2 cc = floor(tc+ofs);"
		"	return texture(Tex, cc/textureSize(Tex, 0)).rgb;"
		"}"

		"vec3 voronoi(vec2 tc)"
		"{"
		"	float md = 2.0;"
		"	int mc = 9;"
		"	for(int c=0; c<9; ++c)"
		"	{"
		"		float d = dist(tc, offs[c]);"
		"		if(md > d)"
		"		{"
		"			md = d;"
		"			mc = c;"
		"		}"
		"	}"
		"	return mix("
		"		point_color(tc, offs[mc])*mix(1.4, 0.5, md),"
		"		vec3(0, 0, 0),"
		"		pow(exp(1-md*512/Scale), 2.0)"
		"	);"
		"}"

		"void main(void)"
		"{"
		"	fragColor = voronoi(vertTexCoord);"
		"}"
		).Compile();

		prog << vs << fs;
		prog.Link().Use();

		return std::move(prog);
	}

	Program& self(void)
	{
		return *this;
	}
public:
	Uniform<Vec2f> offset;
	Uniform<float> scale;

	TexProgram(void)
	 : Program(make())
	 , offset(self(), "Offset")
	 , scale(self(), "Scale")
	{
		UniformSampler(self(), "Tex").Set(0);
	}
};

class VoronoiExample
 : public Example
{
private:
	Context gl;

	TexProgram prog;

	shapes::ShapeWrapper screen;

	Texture tex;
public:
	VoronoiExample(void)
	 : gl()
	 , prog()
	 , screen(List("Position").Get(), shapes::Screen(), prog)
	{
		std::srand(unsigned(std::time(0)));

		tex	<< TextureTarget::_2D
			<< TextureFilter::Nearest
			<< TextureWrap::Repeat
			<< images::RandomRGBUByte(256, 256);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
	}

	void Render(double time)
	{
		float o = 2;
		float s = 24;
		prog.offset.Set(
			GLfloat(CosineWave(time/59.0)*o),
			GLfloat(SineWave(time/61.0)*o)
		);
		prog.scale.Set(GLfloat(s + 1 + SineWave(time / 19.0)*s));

		screen.Draw();
	}

	bool Continue(double time)
	{
		return time < 60.0;
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
	return std::unique_ptr<Example>(new VoronoiExample);
}

} // namespace oglplus
