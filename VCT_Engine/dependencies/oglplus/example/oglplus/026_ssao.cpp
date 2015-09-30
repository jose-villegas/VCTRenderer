/**
 *  @example oglplus/026_ssao.cpp
 *  @brief Shows simple SSAO implementation
 *
 *  @oglplus_screenshot{026_ssao}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_0}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 *  @oglplus_example_uses_model{stanford_dragon}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/obj_mesh.hpp>
#include <oglplus/shapes/screen.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/images/image_spec.hpp>

#include <oglplus/opt/resources.hpp>
#include <oglplus/opt/list_init.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class DataProgram
 : public Program
{
private:
	const Program& prog(void) const { return *this; }

	static Program make(void)
	{
		VertexShader vs;
		vs.Source(
		"#version 150\n"
		"uniform mat4 ModelMatrix;"
		"uniform vec3 LightPosition;"
		"uniform vec3 CameraPosition;"

		"in vec4 Position;"

		"out vec3 vertLightDir;"
		"out vec3 vertViewDir;"

		"void main(void)"
		"{"
		"	gl_Position = ModelMatrix * Position;"
		"	vertLightDir = LightPosition - gl_Position.xyz;"
		"	vertViewDir = CameraPosition - gl_Position.xyz;"
		"}"
		).Compile();

		GeometryShader gs;
		gs.Source(
		"#version 150\n"
		"layout (triangles) in;"
		"layout (triangle_strip, max_vertices=3) out;"

		"uniform mat4 CameraMatrix;"

		"in vec3 vertLightDir[3];"
		"in vec3 vertViewDir[3];"

		"out vec3 geomLightDir;"
		"out vec3 geomViewDir;"
		"out vec3 geomNormal;"

		"void main(void)"
		"{"
		"	geomNormal = normalize("
		"		cross("
		"			gl_in[1].gl_Position.xyz-"
		"			gl_in[0].gl_Position.xyz,"
		"			gl_in[2].gl_Position.xyz-"
		"			gl_in[0].gl_Position.xyz "
		"		)"
		"	);"
		"	for(int v=0; v!=3; ++v)"
		"	{"
		"		gl_Position = CameraMatrix * gl_in[v].gl_Position;"
		"		geomLightDir = vertLightDir[v];"
		"		geomViewDir = vertViewDir[v];"
		"		EmitVertex();"
		"	}"
		"	EndPrimitive();"
		"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
		"#version 150\n"

		"in vec3 geomLightDir;"
		"in vec3 geomViewDir;"
		"in vec3 geomNormal;"

		"out vec4 fragColor;"
		"void main(void)"
		"{"
		"	vec3 Normal = normalize(geomNormal);"
		"	vec3 LightDir = normalize(geomLightDir);"
		"	vec3 LightRefl  = reflect(-LightDir, Normal);"
		"	vec3 ViewDir = normalize(geomViewDir);"

		"	fragColor = vec4("
		"		clamp(dot(Normal, ViewDir), 0, 1),"
		"		clamp(dot(Normal, LightDir),0, 1),"
		"		clamp(dot(ViewDir, LightRefl), 0, 1),"
		"		gl_FragCoord.z"
		"	);"
		"}"
		).Compile();

		Program prog(ObjectDesc("Data"));
		prog << vs << gs << fs;
		prog.Link().Use();
		return std::move(prog);
	}
public:
	Uniform<Mat4f> camera_matrix, model_matrix;
	Uniform<Vec3f> camera_position, light_position;

	DataProgram(void)
	 : Program(make())
	 , camera_matrix(prog(), "CameraMatrix")
	 , model_matrix(prog(), "ModelMatrix")
	 , camera_position(prog(), "CameraPosition")
	 , light_position(prog(), "LightPosition")
	{ }
};

class DrawProgram
 : public Program
{
private:
	const Program& prog(void) const { return *this; }

	static Program make(void)
	{
		VertexShader vs;
		vs.Source(
		"#version 150\n"
		"in vec4 Position;"
		"void main(void)"
		"{"
		"	gl_Position = Position;"
		"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
		"#version 150\n"

		"uniform sampler2DRect DataMap;"
		"uniform float Slider;"

		"uniform vec2 SampleOffs[32];"
		"const int NSamples = 32;"
		"const float InvNSam = 1.0 / NSamples;"

		"in vec4 gl_FragCoord;"

		"out vec3 fragColor;"

		"void main(void)"
		"{"
		"	vec4 FData = texture(DataMap, gl_FragCoord.xy);"
		"	float Mask = ceil(FData.w);"
		"	float Ambi = 1;"
		"	float View = FData.x;"
		"	float Diff = FData.y;"
		"	float Spec = pow(FData.z, 8.0);"

		"	float SampleSpread = mix(16, 8, FData.w);"

		"	float SSAO = 0.0;"
		"	for(int s=0; s!=NSamples; ++s)"
		"	{"
		"		vec2 SampleCoord = gl_FragCoord.xy+SampleSpread*SampleOffs[s];"
		"		vec4 SData = texture(DataMap, SampleCoord);"
		"		float x = (FData.w - SData.w)*16.0;"
		"		float y = View*x*exp(1-abs(x))*min(exp(x), 1.0);"

		"		SSAO += max(1.0-y*2.0, 0.0);"
		"	}"
		"	SSAO *= InvNSam;"
		"	if(gl_FragCoord.x < Slider) SSAO = 1.0;"

		"	Ambi *= (0.00+0.30*SSAO);"
		"	Diff *= (0.05+0.40*SSAO);"
		"	Spec *= (0.05+0.60*SSAO);"

		"	float Sl = int(gl_FragCoord.x) == int(Slider)?1:0;"

		"	vec3 Color = vec3(0.9, 0.8, 0.3)*(Ambi+Diff)+vec3(Spec);"
		"	vec3 BgColor = vec3(0.2);"
		"	vec3 SlColor = vec3(1.0);"
		"	fragColor = mix(mix(BgColor, Color, Mask), SlColor, Sl);"
		"}"
		).Compile();

		Program prog(ObjectDesc("Draw"));
		prog << vs << fs;
		prog.Link().Use();
		return std::move(prog);
	}
public:
	Uniform<GLfloat> slider;

	DrawProgram(void)
	 : Program(make())
	 , slider(prog(), "Slider")
	{
		Uniform<Vec2f> sample_offs(prog(), "SampleOffs");
		for(GLuint i=0; i!=32; ++i)
		{
			float u = std::rand() / float(RAND_MAX);
			float v = std::rand() / float(RAND_MAX);
			float x = float(std::sqrt(v) * std::cos(2*3.1415*u));
			float y = float(std::sqrt(v) * std::sin(2*3.1415*u));
			sample_offs[i].Set(x, y);
		}
	}
};

class Shape
 : public shapes::ShapeWrapper
{
private:
	static shapes::ShapeWrapper load(const char* name, const Program& prog)
	{
		std::ifstream input;
		OpenResourceFile(input, "models", name, ".obj");
		if(!input.good())
			throw std::runtime_error("Error opening file for reading");

		return shapes::ShapeWrapper(
			List("Position").Get(),
			shapes::ObjMesh(
				input,
				shapes::ObjMesh::LoadingOptions(false)
			),
			prog
		);
	}
public:
	Shape(const char* name, const Program& prog)
	 : shapes::ShapeWrapper(load(name, prog))
	{ }
};

class DataBuffer
{
private:
	Texture tex;
	Renderbuffer rbo;
	Framebuffer fbo;
public:
	DataBuffer(const Program& prog, GLuint tex_unit, GLuint width, GLuint height)
	{
		Texture::Active(tex_unit);
		ProgramUniformSampler(prog, "DataMap").Set(tex_unit);

		tex	<< Texture::Target::Rectangle
			<< TextureFilter::Linear
			<< TextureWrap::ClampToEdge
			<< images::ImageSpec(
				width, height,
				Format::RGBA,
				InternalFormat::RGBA32F,
				PixelDataType::Float
			);

		rbo	<< Renderbuffer::Target::Renderbuffer
			<< images::ImageSpec(width, height, InternalFormat::DepthComponent32);

		fbo	<< Framebuffer::Target::Draw
			<< FramebufferAttachment::Color << tex
			<< FramebufferAttachment::Depth << rbo
			<< FramebufferComplete();
	}

	void Resize(GLuint width, GLuint height)
	{
		tex	<< Texture::Target::Rectangle
			<< images::ImageSpec(
				width, height,
				Format::RGBA,
				InternalFormat::RGBA32F,
				PixelDataType::Float
			);

		rbo	<< Renderbuffer::Target::Renderbuffer
			<< images::ImageSpec(width, height, InternalFormat::DepthComponent32);
	}

	void Bind(void)
	{
		fbo.Bind(Framebuffer::Target::Draw);
	}
};

class SubsurfExample : public Example
{
private:
	Context gl;

	DataProgram data_prog;
	DrawProgram draw_prog;

	Shape shape;
	shapes::ShapeWrapper screen;

	DataBuffer data_buffer;

	GLuint width, height;
public:
	SubsurfExample(void)
	 : gl()
	 , shape("stanford_dragon", data_prog)
	 , screen(List("Position").Get(), shapes::Screen(), draw_prog)
	 , data_buffer(draw_prog, 0, 800, 600)
	{
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);
		gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		gl.ClearDepth(1.0f);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;

		gl.Viewport(width, height);
		data_buffer.Resize(width, height);
	}

	void Render(double time)
	{
		GLfloat bs_rad = shape.BoundingSphere().Radius()*1.2f;

		auto light =
			CamMatrixf::Orbiting(
				shape.BoundingSphere().Center(),
				shape.BoundingSphere().Radius()*10.0f,
				FullCircles(time / 23.0),
				Degrees(-CosineWave(time / 31.0) * 80)
			);

		auto camera =
			CamMatrixf::Orbiting(
				shape.BoundingSphere().Center(),
				shape.BoundingSphere().Radius()*
				GLfloat(3.2+SineWave(time / 23.0)*0.8),
				FullCircles(time / 19.0),
				Degrees(SineWave(time / 21.0) * 80)
			);

		GLfloat cam_tgt_dist = Distance(
			shape.BoundingSphere().Center(),
			camera.Position()
		);

		auto cam_proj =
			CamMatrixf::PerspectiveX(
				Degrees(45),
				width, height,
				cam_tgt_dist-bs_rad,
				cam_tgt_dist+bs_rad
			);

		auto model =
			ModelMatrixf::RotationZ(Degrees(SineWave(time / 21.0)*25))*
			ModelMatrixf::Translation(0.0f,-bs_rad*0.25f, 0.0f);

		data_prog.Use();
		data_prog.camera_matrix.Set(cam_proj*camera);
		data_prog.model_matrix.Set(model);
		data_prog.camera_position.Set(camera.Position());
		data_prog.light_position.Set(light.Position());

		data_buffer.Bind();

		gl.Enable(Capability::DepthTest);
		gl.Clear().ColorBuffer().DepthBuffer();

		shape.Use();
		shape.Draw();

		draw_prog.Use();
		draw_prog.slider.Set(GLfloat(CosineWave01(time / 11.0)*width));

		DefaultFramebuffer().Bind(Framebuffer::Target::Draw);

		gl.Disable(Capability::DepthTest);
		gl.Clear().ColorBuffer();

		screen.Use();
		screen.Draw();
	}

	bool Continue(double time)
	{
		return time < 90.0;
	}

	double ScreenshotTime(void) const
	{
		return 29.0;
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
	return std::unique_ptr<Example>(new SubsurfExample);
}

} // namespace oglplus
