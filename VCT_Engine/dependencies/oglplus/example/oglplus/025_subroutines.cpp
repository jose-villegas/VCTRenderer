/**
 *  @example oglplus/025_subroutines.cpp
 *  @brief Shows the usage of subroutines and subroutine uniforms
 *
 *  @oglplus_screenshot{025_subroutines}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_shader_subroutine}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/opt/list_init.hpp>

#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/images/newton.hpp>

#include <oglplus/bound/texture.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class CubeVertShader
 : public VertexShader
{
public:
	CubeVertShader(void)
	 : VertexShader(
		ObjectDesc("Cube vertex"),
		"#version 400\n"
		"uniform vec3 LightPosition;"
		"uniform vec3 CameraPosition;"
		"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"

		"in vec4 Position;"
		"in vec3 Normal;"
		"in vec3 TexCoord;"

		"out vec3 vertNormal;"
		"out vec3 vertLightDir;"
		"out vec3 vertLightRefl;"
		"out vec3 vertViewDir;"
		"out vec2 vertTexCoord;"
		"void main(void)"
		"{"
		"	gl_Position = ModelMatrix* Position;"
		"	vertNormal = mat3(ModelMatrix)*Normal;"
		"	vertLightDir = normalize(LightPosition - gl_Position.xyz);"
		"	vertLightRefl = reflect(-vertLightDir, vertNormal);"
		"	vertViewDir = normalize(CameraPosition - gl_Position.xyz);"
		"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
		"	vertTexCoord = TexCoord.xy;"
		"}"
	)
	{ }
};

class CubeFragShader
 : public FragmentShader
{
public:
	CubeFragShader(void)
	 : FragmentShader(
		ObjectDesc("Cube fragment"),
		"#version 400\n"

		"subroutine vec3 pixelColorFuncType(vec2 TexCoord);"

		"subroutine vec3 pixelLightFuncType("
		"	vec3 Color, "
		"	float Ambient, "
		"	float Diffuse, "
		"	float Specular"
		");"


		"uniform vec3 Color1, Color2;"
		"uniform vec2 TexScale;"
		"uniform float SpecularFactor;"
		"uniform sampler2D CubeTex;"
		"subroutine uniform pixelColorFuncType pixelColorFunc;"
		"subroutine uniform pixelLightFuncType pixelLightFunc;"

		"in vec3 vertNormal;"
		"in vec3 vertLightDir;"
		"in vec3 vertLightRefl;"
		"in vec3 vertViewDir;"
		"in vec2 vertTexCoord;"

		"out vec3 fragColor;"

		"const vec3 LightColor = vec3(1.0, 1.0, 1.0);"

		"subroutine(pixelLightFuncType)"
		"vec3 shiny("
		"	vec3 Color, "
		"	float Ambient, "
		"	float Diffuse, "
		"	float Specular"
		")"
		"{"
		"	return (Ambient+Diffuse)*Color +"
		"		Specular * LightColor;"
		"}"

		"subroutine(pixelLightFuncType)"
		"vec3 dull("
		"	vec3 Color, "
		"	float Ambient, "
		"	float Diffuse, "
		"	float Specular"
		")"
		"{"
		"	return (Ambient+Diffuse*0.9)*Color +"
		"		Specular*0.1 * LightColor;"
		"}"

		"subroutine(pixelColorFuncType)"
		"vec3 checker(vec2 TexCoord)"
		"{"
		"	float a = ("
		"		int(TexCoord.x)%2+"
		"		int(TexCoord.y)%2 "
		"	)%2;"
		"	return mix(Color1, Color2, a);"
		"}"

		"subroutine(pixelColorFuncType)"
		"vec3 strips(vec2 TexCoord)"
		"{"
		"	float a = int(TexCoord.x+TexCoord.y)%2;"
		"	return mix(Color1, Color2, a);"
		"}"


		"subroutine(pixelColorFuncType)"
		"vec3 spiral(vec2 TexCoord)"
		"{"
		"	vec2  center = (TexCoord - vec2(0.5, 0.5)) * 16.0;"
		"	float l = length(center);"
		"	float t = atan(center.y, center.x)/(2.0*asin(1.0));"
		"	float m = int(l+t) % 2;"
		"	return mix(Color1, Color2, m);"
		"}"

		"subroutine(pixelColorFuncType)"
		"vec3 textureRGB(vec2 TexCoord)"
		"{"
		"	return texture(CubeTex, TexCoord).rgb;"
		"}"

		"subroutine(pixelColorFuncType)"
		"vec3 textureBGR(vec2 TexCoord)"
		"{"
		"	return texture(CubeTex, TexCoord).bgr;"
		"}"

		"void main(void)"
		"{"
		"	float Ambient = 0.5;"

		"	float Diffuse = max(dot("
		"		normalize(vertNormal),"
		"		normalize(vertLightDir)"
		"	)+0.1, 0.0);"

		"	float Specular = pow(clamp(dot("
		"		normalize(vertViewDir),"
		"		normalize(vertLightRefl)"
		"	), 0.0, 1.0), SpecularFactor);"

		"	fragColor = pixelLightFunc("
		"		pixelColorFunc(vertTexCoord * TexScale),"
		"		Ambient, "
		"		Diffuse, "
		"		Specular "
		"	);"
		"}"
	)
	{ }
};

class CubeProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Cube program"));
		prog.AttachShader(CubeVertShader());
		prog.AttachShader(CubeFragShader());
		prog.Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix, model_matrix;
	ProgramUniform<Vec3f> camera_position, light_position;
	ProgramUniform<Vec3f> color_1, color_2;
	ProgramUniform<Vec2f> tex_scale;
	ProgramUniform<GLfloat> specular_factor;
	ProgramUniformSampler cube_tex;

	SubroutineUniform pixel_light_func, pixel_color_func;

	Subroutine shiny, dull;
	Subroutine checker, strips, spiral, texture_rgb, texture_bgr;

	UniformSubroutines frag_subroutines;

	UniformSubroutines::Preset frag_shiny_checker;
	UniformSubroutines::Preset frag_shiny_texture;
	UniformSubroutines::Preset frag_shiny_strips;
	UniformSubroutines::Preset frag_shiny_spiral;

	CubeProgram(void)
	 : Program(make())
	 , projection_matrix(prog(), "ProjectionMatrix")
	 , camera_matrix(prog(), "CameraMatrix")
	 , model_matrix(prog(), "ModelMatrix")
	 , camera_position(prog(), "CameraPosition")
	 , light_position(prog(), "LightPosition")
	 , color_1(prog(), "Color1")
	 , color_2(prog(), "Color2")
	 , tex_scale(prog(), "TexScale")
	 , specular_factor(prog(), "SpecularFactor")
	 , cube_tex(prog(), "CubeTex")
	 , pixel_light_func(prog(), ShaderType::Fragment, "pixelLightFunc")
	 , pixel_color_func(prog(), ShaderType::Fragment, "pixelColorFunc")
	 , shiny(prog(), ShaderType::Fragment, "shiny")
	 , dull(prog(), ShaderType::Fragment, "dull")
	 , checker(prog(), ShaderType::Fragment, "checker")
	 , strips(prog(), ShaderType::Fragment, "strips")
	 , spiral(prog(), ShaderType::Fragment, "spiral")
	 , texture_rgb(prog(), ShaderType::Fragment, "textureRGB")
	 , texture_bgr(prog(), ShaderType::Fragment, "textureBGR")
	 , frag_subroutines(prog(), ShaderType::Fragment)
	 , frag_shiny_checker(
		frag_subroutines
			.Assign(pixel_light_func, shiny)
			.Assign(pixel_color_func, checker)
			.Save()
	), frag_shiny_texture(
		frag_subroutines
			.Assign(pixel_color_func, texture_rgb)
			.Save()
	), frag_shiny_strips(
		frag_subroutines
			.Assign(pixel_color_func, strips)
			.Save()
	), frag_shiny_spiral(
		frag_subroutines
			.Assign(pixel_color_func, spiral)
			.Save()
	)
	{ }
};

class CubeExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	// the cube shading program
	CubeProgram cube_prog;

	// texture applied to the cube
	Texture cube_tex;

	// wraper for the cube builder, VAO and VBOs
	shapes::ShapeWrapper cube;
public:
	CubeExample(void)
	 : cube(
		List("Position")("Normal")("TexCoord").Get(),
		shapes::Cube(),
		cube_prog
	)
	{
		// setup the texture
		{
			GLuint tex_side = 512;
			auto image = images::NewtonFractal(
				tex_side, tex_side,
				Vec3f(0.2f, 0.1f, 0.4f),
				Vec3f(0.8f, 0.8f, 1.0f),
				Vec2f(-1.0f, -1.0f),
				Vec2f( 1.0f,  1.0f),
				images::NewtonFractal::X4Minus1(),
				images::NewtonFractal::DefaultMixer()
			);

			gl.Bound(Texture::Target::_2D, cube_tex)
				.Image2D(image)
				.GenerateMipmap()
				.BorderColor(Vec4f(0.8f, 0.8f, 1.0f, 1.0f))
				.MinFilter(TextureMinFilter::LinearMipmapLinear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::Repeat)
				.WrapT(TextureWrap::Repeat);
		}

		cube_prog.cube_tex = 0;
		cube_prog.light_position.Set(4.0f, 4.0f, -8.0f);

		gl.ClearColor(0.8f, 0.8f, 0.7f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		cube_prog.projection_matrix = CamMatrixf::PerspectiveX(
			Degrees(60),
			float(width)/height,
			1, 25
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		auto camera = CamMatrixf::Orbiting(
			Vec3f(0.0, 1.5, 0.0),
			GLfloat(7.0 + SineWave(time / 11.0)*1.5),
			FullCircles(time / 19.0),
			Degrees(SineWave(time / 20.0) * 30 + 35)
		);
		cube_prog.camera_matrix = camera;
		cube_prog.camera_position = camera.Position();


		// shiny gray/blue checkered cube
		cube_prog.frag_subroutines.Apply(cube_prog.frag_shiny_checker);

		cube_prog.specular_factor = 32;
		cube_prog.color_1 = Vec3f(0.9f, 0.8f, 0.7f);
		cube_prog.color_2 = Vec3f(0.3f, 0.4f, 0.5f);
		cube_prog.tex_scale = Vec2f(4, 4);
		cube_prog.model_matrix =
			ModelMatrixf::RotationY(FullCircles(time / 7.0))*
			ModelMatrixf::Translation( 2.0f, 0.0f, 0.0f)*
			ModelMatrixf::RotationX(Degrees(25 * time));
		cube.Draw();

		// shiny textured cube
		cube_prog.frag_subroutines.Apply(cube_prog.frag_shiny_texture);

		cube_prog.specular_factor = 16;
		cube_prog.tex_scale = Vec2f(1, 1);
		cube_prog.model_matrix =
			ModelMatrixf::RotationY(FullCircles(time / 7.0))*
			ModelMatrixf::Translation(-2.0f, 0.0f, 0.0f)*
			ModelMatrixf::RotationX(Degrees(-17 * time));
		cube.Draw();

		// shiny yellow/black striped cube
		cube_prog.frag_subroutines.Apply(cube_prog.frag_shiny_strips);

		cube_prog.specular_factor = 32;
		cube_prog.color_1 = Vec3f(0.9f, 0.9f, 0.1f);
		cube_prog.color_2 = Vec3f(0.1f, 0.1f, 0.1f);
		cube_prog.tex_scale = Vec2f(16, 16);
		cube_prog.model_matrix =
			ModelMatrixf::RotationY(FullCircles(time / 7.0))*
			ModelMatrixf::Translation( 0.0f, 2.0f, 0.0f)*
			ModelMatrixf::RotationY(Degrees(37 * time));
		cube.Draw();

		// shiny gray/green spiral cube
		cube_prog.frag_subroutines.Apply(cube_prog.frag_shiny_spiral);

		cube_prog.specular_factor = 24;
		cube_prog.color_1 = Vec3f(0.9f, 0.9f, 0.9f);
		cube_prog.color_2 = Vec3f(0.4f, 0.9f, 0.4f);
		cube_prog.tex_scale = Vec2f(1, 1);
		cube_prog.model_matrix =
			ModelMatrixf::RotationY(FullCircles(time / 7.0))*
			ModelMatrixf::Translation( 0.0f,-2.0f, 0.0f)*
			ModelMatrixf::RotationY(Degrees(-13 * time));
		cube.Draw();

		// dull white/red striped cube
		cube_prog.frag_subroutines
			.Assign(
				cube_prog.pixel_light_func,
				cube_prog.dull
			).Assign(
				cube_prog.pixel_color_func,
				cube_prog.strips
			).Apply();

		cube_prog.specular_factor = 32;
		cube_prog.color_2 = Vec3f(1.0f, 1.0f, 1.0f);
		cube_prog.color_1 = Vec3f(0.9f, 0.2f, 0.2f);
		cube_prog.tex_scale = Vec2f(8, 6);
		cube_prog.model_matrix =
			ModelMatrixf::RotationY(FullCircles(time / 7.0))*
			ModelMatrixf::Translation( 0.0, 0.0, 2.0)*
			ModelMatrixf::RotationZ(Degrees(27 * time));
		cube.Draw();

		// dull textured cube
		cube_prog.frag_subroutines
			.Assign(
				cube_prog.pixel_color_func,
				cube_prog.texture_bgr
			).Apply();

		cube_prog.tex_scale = Vec2f(1, 1);
		cube_prog.model_matrix =
			ModelMatrixf::RotationY(FullCircles(time / 7.0))*
			ModelMatrixf::Translation( 0.0, 0.0,-2.0)*
			ModelMatrixf::RotationZ(Degrees(-23 * time));
		cube.Draw();
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
	return std::unique_ptr<Example>(new CubeExample);
}

} // namespace oglplus
