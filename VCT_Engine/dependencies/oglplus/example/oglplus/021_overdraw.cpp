/**
 *  @example oglplus/021_overdraw.cpp
 *  @brief Shows a visualisation of framebuffer overdraw
 *
 *  @oglplus_screenshot{021_overdraw}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{INITIALIZER_LISTS}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/dsa/ext/buffer.hpp>
#include <oglplus/dsa/ext/framebuffer.hpp>
#include <oglplus/dsa/ext/renderbuffer.hpp>
#include <oglplus/dsa/ext/texture.hpp>
#include <oglplus/dsa/uniform.hpp>

#include <oglplus/images/image_spec.hpp>
#include <oglplus/images/gradient.hpp>

#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/screen.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/opt/list_init.hpp>

#include <map>

#include "example.hpp"

namespace oglplus {

class DrawProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		VertexShader vs;
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"layout (std140) uniform OffsetBlock {vec3 Offset[16*16*16];};"
			"in vec3 Position;"
			"void main(void)"
			"{"
			"	gl_Position = "
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		vec4(Position+Offset[gl_InstanceID], 1.0);"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 140\n"
			"out float fragValue;"
			"void main(void)"
			"{"
			"	fragValue = 1.0/16.0;"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();

		return std::move(prog);
	}

	Program& self(void) { return *this; }
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix;

	DrawProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	{ }
};

class ScreenProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		VertexShader vs;
		vs.Source(
			"#version 140\n"
			"uniform vec2 ScreenSize;"
			"in vec4 Position;"
			"in vec2 TexCoord;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"	vertTexCoord = TexCoord*ScreenSize;"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 140\n"
			"uniform sampler1D Palette;"
			"uniform sampler2DRect Tex;"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float Overdraw = texture(Tex, vertTexCoord).r;"
			"	fragColor = texture(Palette, Overdraw);"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs).Link().Use();
		return std::move(prog);
	}

	Program& self(void) { return *this; }
public:
	ProgramUniform<Vec2f> screen_size;

	ScreenProg(void)
	 : Program(make())
	 , screen_size(self(), "ScreenSize")
	{ }
};

struct OffsetData
 : public std::vector<GLfloat>
{
	OffsetData(GLuint n)
	 : std::vector<GLfloat>(n*n*n*4)
	{
		GLfloat d = 1.414f;
		auto p = begin();

		for(GLuint k=0; k!=n; ++k)
		{
			GLfloat z = (k-(n+1)*0.5f)*d;
			for(GLuint j=0; j!=n; ++j)
			{
				GLfloat y = (j-(n+1)*0.5f)*d;
				for(GLuint i=0; i!=n; ++i)
				{
					GLfloat x = (i-(n+1)*0.5f)*d;
					(*p++) = x;
					(*p++) = y;
					(*p++) = z;
					(*p++) = 0;
				}
			}
		}
	}
};

class CubeExample : public Example
{
private:
	const GLuint n;
	Context gl;

	ScreenProg screen_prog;
	DrawProg draw_prog;

	shapes::ShapeWrapper screen, cube;
	DSABufferEXT cube_pos;

	DSATextureEXT palette, tex;
	DSAFramebufferEXT fbo;
	DSARenderbufferEXT rbo;
	DefaultFramebuffer dfb;
public:
	CubeExample(void)
	 : n(16)
	 , screen_prog()
	 , draw_prog()
	 , screen(List("Position")("TexCoord").Get(), shapes::Screen(), screen_prog)
	 , cube(List("Position").Get(), shapes::Cube(), draw_prog)
	{

		draw_prog.Use();

		UniformBlock(draw_prog, "OffsetBlock").Binding(0);
		cube_pos<< BufferIndexedTarget::Uniform << 0
			<< BufferUsage::StaticDraw
			<< OffsetData(n);

		ProgramUniformSampler(screen_prog, "Palette").Set(0);
		Texture::Active(0);
		palette	<< Texture::Target::_1D
			<< TextureFilter::Nearest
			<< TextureWrap::ClampToEdge
			<< images::LinearGradient(
				16,
				Vec3f(0, 0, 0),
				std::map<GLfloat, Vec3f>({
					{  0.f/16.f, Vec3f(0.0f, 0.0f, 0.0f)},
					{  1.f/16.f, Vec3f(0.5f, 0.0f, 1.0f)},
					{  3.f/16.f, Vec3f(0.0f, 0.0f, 1.0f)},
					{  6.f/16.f, Vec3f(0.0f, 0.6f, 0.6f)},
					{  8.f/16.f, Vec3f(0.0f, 1.0f, 0.0f)},
					{ 11.f/16.f, Vec3f(0.6f, 0.6f, 0.0f)},
					{ 13.f/16.f, Vec3f(1.0f, 0.1f, 0.0f)},
					{ 16.f/16.f, Vec3f(0.7f, 0.0f, 0.0f)}
				})
			);

		ProgramUniformSampler(screen_prog, "Tex").Set(1);
		Texture::Active(1);
		tex	<< Texture::Target::Rectangle
			<< TextureMinFilter::Nearest
			<< TextureMagFilter::Nearest
			<< TextureWrap::ClampToEdge
			<< images::ImageSpec(
				64, 64,
				Format::Red,
				InternalFormat::R8,
				DataType::UnsignedByte
			);

		rbo	<< Renderbuffer::Target::Renderbuffer
			<< images::ImageSpec(64, 64, InternalFormat::DepthComponent);

		fbo	<< Framebuffer::Target::Draw
			<< FramebufferAttachment::Color << tex
			<< FramebufferAttachment::Depth << rbo
			<< FramebufferComplete();

		gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::CullFace);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		draw_prog.projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(70),
				width, height,
				1, 100
			)
		);
		screen_prog.screen_size.Set(width, height);

		tex.Image2D(
			0,
			InternalFormat::R8,
			width, height,
			0,
			Format::Red,
			DataType::UnsignedByte,
			nullptr
		);
		rbo.Storage(InternalFormat::DepthComponent, width, height);
	}

	void RenderOffscreen(double time)
	{
		fbo.Bind(Framebuffer::Target::Draw);

		gl.Clear().ColorBuffer().DepthBuffer();

		gl.BlendFunc(BlendFn::One, BlendFn::One);
		gl.Enable(Capability::Blend);
		gl.Enable(Capability::DepthTest);

		draw_prog.Use();
		draw_prog.camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				27.0,
				Degrees(time * 23),
				Degrees(SineWave(time / 23.0) * 80)
			)
		);

		cube.Use();
		cube.Draw(n*n*n);

		gl.Disable(Capability::DepthTest);
		gl.Disable(Capability::Blend);

		dfb.Bind(Framebuffer::Target::Draw);
	}

	void RenderOnscreen(double)
	{
		screen_prog.Use();
		screen.Use();
		screen.Draw();
	}

	void Render(double time)
	{
		RenderOffscreen(time);
		RenderOnscreen(time);
	}

	double ScreenshotTime(void) const
	{
		return 8.1;
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
