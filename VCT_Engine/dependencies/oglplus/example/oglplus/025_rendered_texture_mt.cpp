/**
 *  @example oglplus/025_rendered_texture_mt.cpp
 *  @brief Shows how to render into a texture
 *
 *  @oglplus_screenshot{025_rendered_texture_mt}
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{THREADS}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_2}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/twisted_torus.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>
#include <oglplus/bound/renderbuffer.hpp>

#include <oglplus/opt/list_init.hpp>

#include "example.hpp"
#include "example_mt.hpp"

namespace oglplus {

class FBTexThread;

class FBTexExample : public Example
{
private:
	friend class FBTexThread;

	Context gl;

	VertexShader vertex_shader;

	Program prog;

	Program make_prog(void)
	{
		Program result(ObjectDesc("Main"));

		vertex_shader.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec2 TexCoord;"
			"out vec3 vertNormal;"
			"out vec3 vertLight;"
			"out vec2 vertTexCoord;"
			"uniform vec3 LightPos;"
			"void main(void)"
			"{"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	gl_Position = ModelMatrix * Position;"
			"	vertLight = LightPos-gl_Position.xyz;"
			"	vertTexCoord = TexCoord;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		).Compile();
		result.AttachShader(vertex_shader);

		FragmentShader fs(ObjectDesc("Main fragment"));
		fs.Source(
			"#version 140\n"
			"uniform sampler2D TexUnit;"
			"in vec3 vertNormal;"
			"in vec3 vertLight;"
			"in vec2 vertTexCoord;"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float d = dot(vertNormal, normalize(vertLight));"
			"	float i = 0.6 + max(d, 0.0);"
			"	fragColor = texture(TexUnit, vertTexCoord)*i;"
			"}"
		);
		fs.Compile();
		result.AttachShader(fs);

		result.Link().Use();

		return std::move(result);
	}

	// Handle for matrix uniforms
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	shapes::ShapeWrapper cube;

	GLuint width, height;

	Texture tex;

	DefaultFramebuffer dfb;

	ExampleSyncQueue  parent_ready;
	ExampleSyncQueue* thread_ready;
public:
	FBTexExample(void)
	 : gl()
	 , vertex_shader(ObjectDesc("Vertex"))
	 , prog(make_prog())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	 , cube(List("Position")("Normal")("TexCoord").Get(), shapes::Cube(), prog)
	 , width(800)
	 , height(600)
	{
		UniformSampler(prog, "TexUnit").Set(0);
		Uniform<Vec3f>(prog, "LightPos").Set(40.0f, 40.0f, -80.0f);

		Texture::Active(0);
		tex.Bind(Texture::Target::_2D);

		Use();
	}

	void SetProjection(void)
	{
		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(54),
				width, height,
				1, 100
			)
		);
	}

	void Use(void)
	{
		gl.ClearDepth(1.0f);
		gl.ClearColor(0.8f, 0.8f, 0.8f, 0.0f);

		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);

		dfb.Bind(Framebuffer::Target::Draw);
		gl.Viewport(width, height);

		prog.Use();
		cube.Use();

		SetProjection();
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;

		gl.Viewport(width, height);
		SetProjection();
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				3.0,
				Degrees(time * 35),
				Degrees(SineWave(time / 20.0) * 60)
			)
		);

		model_matrix.Set(ModelMatrixf::RotationX(FullCircles(time * 0.25)));

		assert(thread_ready);
		thread_ready->Wait();

		cube.Draw();

		parent_ready.Signal();
	}

	bool Continue(double time)
	{
		return time < 30.0;
	}

	double ScreenshotTime(void) const
	{
		return 2.0;
	}
};

class FBTexThread
 : public ExampleThread
{
private:
	Context gl;

	Program prog;

	static Program make_prog(FBTexExample& example)
	{
		Program prog(ObjectDesc("Thread"));

		prog.AttachShader(example.vertex_shader);

		FragmentShader fs(ObjectDesc("Thread fragment"));
		fs.Source(
			"#version 140\n"
			"in vec3 vertNormal;"
			"in vec3 vertLight;"
			"in vec2 vertTexCoord;"
			"out vec3 fragColor;"
			"void main(void)"
			"{"
			"	float d = max(dot("
			"		vertNormal, "
			"		normalize(vertLight)"
			"	), 0.0);"
			"	float i = int("
			"		vertTexCoord.x*18.0+"
			"		vertTexCoord.y*14.0 "
			"	) % 2;"
			"	vec3 Red =   vec3(1.0, 0.2, 0.2);"
			"	vec3 White = vec3(1.0, 1.0, 1.0);"
			"	fragColor = (0.4+0.8*d)*mix(Red, White, i);"
			"}"
		);
		fs.Compile();
		prog.AttachShader(fs);

		prog.Link().Use();

		return std::move(prog);
	}

	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	shapes::ShapeWrapper shape;

	Framebuffer fbo;
	Renderbuffer rbo;

	GLuint tex_side;

	ExampleSyncQueue  thread_ready;
	ExampleSyncQueue& parent_ready;
public:
	FBTexThread(FBTexExample& example)
	 : gl()
	 , prog(make_prog(example))
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	 , shape(List("Position")("Normal")("TexCoord").Get(), shapes::TwistedTorus(), prog)
	 , tex_side(512)
	 , parent_ready(example.parent_ready)
	{
		example.thread_ready = &thread_ready;

		Uniform<Vec3f>(prog, "LightPos").Set(20.0f, 30.0f, 40.0f);

		Texture::Active(0);
		gl.Bound(Texture::Target::_2D, example.tex)
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::Repeat)
			.WrapT(TextureWrap::Repeat)
			.Image2D(
				0,
				PixelDataInternalFormat::RGBA,
				tex_side, tex_side,
				0,
				PixelDataFormat::RGBA,
				PixelDataType::UnsignedByte,
				nullptr
			);

		gl.Bound(Renderbuffer::Target::Renderbuffer, rbo)
 			.Storage(
				PixelDataInternalFormat::DepthComponent,
				tex_side,
				tex_side
			);

		gl.Bound(Framebuffer::Target::Draw, fbo)
			.AttachTexture(FramebufferAttachment::Color, example.tex, 0)
			.AttachRenderbuffer(FramebufferAttachment::Depth, rbo);

		Use();
	}

	void Use(void)
	{
		gl.ClearDepth(1.0f);
		gl.ClearColor(0.9f, 0.4f, 0.4f, 1.0f);

		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);

		fbo.Bind(Framebuffer::Target::Draw);
		gl.Viewport(tex_side, tex_side);

		prog.Use();
		shape.Use();

		projection_matrix.Set(CamMatrixf::PerspectiveX(Degrees(48), 1.0, 1, 100));
	}

	void Cancel(void)
	{
		parent_ready.Cancel();
		thread_ready.Cancel();
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();

		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				4.0,
				Degrees(time * 25),
				Degrees(SineWave(time / 30.0) * 90)
			)
		);

		model_matrix.Set(
			ModelMatrixf::RotationA(
				Vec3f(1.0f, 1.0f, 1.0f),
				FullCircles(time * 0.5)
			)
		);

		shape.Draw();

		thread_ready.Signal();
		parent_ready.Wait();
	}
};

void setupExample(ExampleParams& params)
{
	if(params.max_threads > 0)
		params.num_threads = 1;
	else throw std::runtime_error(
		"This example requires multi-threading"
	);
}

std::unique_ptr<ExampleThread> makeExampleThread(
	Example& example,
	unsigned thread_id,
	const ExampleParams& /*params*/
)
{
	assert(thread_id == 0);

	return std::unique_ptr<ExampleThread>(
		new FBTexThread(
			dynamic_cast<FBTexExample&>(example)
		)
	);
}

std::unique_ptr<Example> makeExample(const ExampleParams& /*params*/)
{
	return std::unique_ptr<Example>(new FBTexExample);
}

} // namespace oglplus
