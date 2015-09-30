/**
 *  @example oglplus/030_pin_display.cpp
 *  @brief Shows how to render a pin display
 *
 *  @oglplus_screenshot{030_pin_display}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/dsa/ext/texture.hpp>
#include <oglplus/dsa/ext/framebuffer.hpp>

#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/shapes/cage.hpp>
#include <oglplus/shapes/icosahedron.hpp>
#include <oglplus/shapes/twisted_torus.hpp>
#include <oglplus/shapes/wicker_torus.hpp>
#include <oglplus/shapes/spiral_sphere.hpp>

#include <oglplus/images/sphere_bmap.hpp>

#include <oglplus/opt/list_init.hpp>
#include <oglplus/opt/resources.hpp>

#include <list>

#include "example.hpp"

namespace oglplus {

class ShadowProg
 : public Program
{
public:
	static Program make(void)
	{
		Program result;

		VertexShader vs;
		vs.Source(
			"#version 140\n"
			"#define side 128\n"

			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"uniform float Fade;"
			"uniform sampler2D Offsets;"
			"uniform sampler2D Heights;"

			"in vec4 Position;"

			"void main(void)"
			"{"
			"	ivec2 Coord = ivec2(gl_InstanceID%side, gl_InstanceID/side);"
			"	vec2 Offs = texelFetch(Offsets, Coord, 0).xy;"
			"	float Height = 1.0-texelFetch(Heights, Coord, 0).r;"
			"	gl_Position = Position;"
			"	gl_Position.xz += Offs;"
			"	gl_Position.y  *= max(Height*Fade*side/2, 0.5);"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 140\n"
			"void main(void) { }"
		).Compile();

		result.AttachShader(vs).AttachShader(fs);
		result.Link().Validate().Use();
		return std::move(result);
	}

	Program& self(void) { return *this; }
public:

	ProgramUniform<Mat4f> projection_matrix, camera_matrix;
	ProgramUniform<GLfloat> fade;

	ShadowProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , fade(self(), "Fade")
	{ }
};

class DisplayProg
 : public Program
{
public:
	static Program make(void)
	{
		Program result;

		VertexShader vs;
		vs.Source(
			"#version 140\n"
			"#define side 128\n"

			"uniform mat4 ProjectionMatrix, CameraMatrix, LightProjMatrix, LightMatrix;"
			"uniform vec3 CameraPos, LightPos;"
			"uniform float Fade;"
			"uniform sampler2D Offsets;"
			"uniform sampler2D Heights;"

			"in vec4 Position;"
			"in vec3 Normal;"

			"out vec4 vertShadowCoord;"
			"out vec3 vertViewDir;"
			"out vec3 vertLightDir;"
			"out vec3 vertNormal;"
			"out float vertLight;"

			"void main(void)"
			"{"
			"	ivec2 Coord = ivec2(gl_InstanceID%side, gl_InstanceID/side);"
			"	vec2 Offs = texelFetch(Offsets, Coord, 0).xy;"
			"	float Depth = texelFetch(Heights, Coord, 0).r;"
			"	float Height = 1.0-Depth;"
			"	gl_Position = Position;"
			"	gl_Position.xz += Offs;"
			"	float l = (1.0-dot(Normal, vec3(0,1,0))*0.5)*0.8;"
			"	vertLight = (1.0-gl_Position.y*l)*sign(Height)*Fade;"
			"	gl_Position.y  *= max(Height*Fade*side/2, 0.5);"
			"	vertViewDir = CameraPos - gl_Position.xyz;"
			"	vertLightDir = LightPos - gl_Position.xyz;"
			"	vertShadowCoord = LightProjMatrix * LightMatrix * gl_Position;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"	vertNormal = Normal;"
			"}"
		).Compile();

		FragmentShader fs;
		fs.Source(
			"#version 140\n"
			"uniform sampler2DShadow Shadows;"

			"const vec3 LightDir = normalize(vec3(1, 0.3, 1));"
			"const vec3 BarColor = vec3(0.4, 0.4, 0.4);"

			"in vec4 vertShadowCoord;"
			"in vec3 vertViewDir;"
			"in vec3 vertLightDir;"
			"in vec3 vertNormal;"
			"in float vertLight;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	vec3 Normal = normalize(vertNormal);"
			"	vec3 ViewDir = normalize(vertViewDir);"
			"	vec3 LightDir = normalize(vertLightDir);"
			"	vec3 LightRefl = reflect(-LightDir, Normal);"
			"	vec3 ShadowCoord = (vertShadowCoord.xyz/vertShadowCoord.w)*0.5 + 0.5;"
			"	float shdw = texture(Shadows, ShadowCoord);"
			"	float ambi = 0.15;"
			"	float diff = pow(max(dot(Normal, LightDir)+0.1, 0.0),2.0)*0.9;"
			"	float spec = pow(max(dot(ViewDir, LightRefl), 0.0), 32.0)*0.4;"
			"	float emis = pow(vertLight, 2.0)*0.7;"
			"	fragColor = "
			"		BarColor * (diff*shdw+ambi)+"
			"		vec3(1.0, 1.0, 1.0)*spec*shdw+"
			"		vec3(0.1, 1.0, 0.3)*emis;"
			"}"
		).Compile();

		result.AttachShader(vs).AttachShader(fs);
		result.Link().Validate().Use();
		return std::move(result);
	}

	Program& self(void) { return *this; }
public:

	ProgramUniform<Mat4f> projection_matrix, camera_matrix;
	ProgramUniform<Mat4f> light_proj_matrix, light_matrix;
	ProgramUniform<Vec3f> camera_pos, light_pos;
	ProgramUniform<GLfloat> fade;

	DisplayProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , light_proj_matrix(self(), "LightProjMatrix")
	 , light_matrix(self(), "LightMatrix")
	 , camera_pos(self(), "CameraPos")
	 , light_pos(self(), "LightPos")
	 , fade(self(), "Fade")
	{ }
};

class DisplayScene
{
private:
	Context gl;

	Program prog;

	static Program make_prog(void)
	{
		Program prog;

		VertexShader vs;
		vs.Source(
			"#version 140\n"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix;"
			"mat4 Matrix = ProjectionMatrix*CameraMatrix*ModelMatrix;"
			"in vec4 Position;"

			"void main(void)"
			"{"
			"	gl_Position = Matrix*Position;"
			"}"
		);

		FragmentShader fs;
		fs.Source(
			"#version 140\n"
			"void main(void){ }"
		);

		prog.AttachShader(vs).AttachShader(fs);
		prog.Link().Validate().Use();

		return std::move(prog);
	}

	ProgramUniform<Mat4f> projection_matrix, camera_matrix, model_matrix;

	std::list<shapes::ShapeWrapper> shapes;
	std::list<shapes::ShapeWrapper>::iterator ishape;
	double shape_time;
public:
	DisplayScene(void)
	 : gl()
	 , prog(make_prog())
	 , projection_matrix(prog, "ProjectionMatrix")
	 , camera_matrix(prog, "CameraMatrix")
	 , model_matrix(prog, "ModelMatrix")
	 , ishape(shapes.end())
	 , shape_time(0)
	{ }

	template <typename ShapeGenerator>
	void AddShape(const ShapeGenerator& shape_gen)
	{
		shapes.emplace_back(
			List("Position").Get(),
			shape_gen,
			prog
		);
		if(shapes.size() == 1)
		{
			ishape = shapes.begin();
		}
	}

	double Draw(double time)
	{
		assert(!shapes.empty());
		assert(ishape != shapes.end());
		shapes::ShapeWrapper& shape = *ishape;


		const double interval = 11.0;
		double segment = time - shape_time;
		double fade = segment*(interval-segment);
		fade -= 1.0;
		if(fade < 0.0) fade = 0.0;
		fade = std::sqrt(fade/interval);
		if(fade > 1.0) fade = 1.0;

		if(segment > interval)
		{
			if(++ishape == shapes.end())
			{
				ishape = shapes.begin();
			}
			shape_time = time;
		}

		gl.Clear().DepthBuffer();

		float dist = float((1.0+SineWave(time / 13.0))*2.5);

		projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(45),
				1.0f,
				1.0f+dist,
				shape.BoundingSphere().Radius()*2.0f+1.0f+dist
			)
		);

		camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				GLfloat(shape.BoundingSphere().Radius()+1.5+dist),
				FullCircles(time / 27.0),
				Degrees(SineWave(time / 23.0) * 89)
			)
		);
		model_matrix.Set(
			ModelMatrixf::RotationA(
				Vec3f(1,1,1),
				FullCircles(time /-37.0)
			)
		);

		prog.Use();
		shape.Use();
		shape.Draw();

		return fade;
	}
};

class BarDisplay
{
private:
	Context gl;

	const GLuint side;
	const GLuint shadow_size;
	GLuint width, height;

	DisplayProg display_prog;
	ShadowProg shadow_prog;

	shapes::ShapeWrapper cube;
	VertexArray display_vao, shadow_vao;

	DSATextureEXT offsets;
	DSATextureEXT heights;
	DSATextureEXT shadows;

	DSAFramebufferEXT heights_fbo;
	DSAFramebufferEXT shadows_fbo;
	DefaultFramebuffer default_fb;

	void init_offsets(void)
	{
		std::vector<GLfloat> offset_data(side*side*2);

		auto p = offset_data.begin();

		for(GLuint j=0; j!=side; ++j)
		{
			GLfloat v = j/GLfloat(side-1) - 0.5f;
			for(GLuint i=0; i!=side; ++i)
			{
				GLfloat u = i/GLfloat(side-1) - 0.5f;
				*p++ = u*side;
				*p++ = v*side;
			}
		}

		ProgramUniformSampler(display_prog, "Offsets").Set(0);
		ProgramUniformSampler(shadow_prog, "Offsets").Set(0);
		offsets.target = Texture::Target::_2D;
		offsets.BindMulti(0, Texture::Target::_2D);
		offsets.Image2D(
			0,
			PixelDataInternalFormat::RG32F,
			side, side,
			0,
			PixelDataFormat::RG,
			PixelDataType::Float,
			offset_data.data()
		);
		offsets.MinFilter(TextureMinFilter::Nearest);
		offsets.MagFilter(TextureMagFilter::Nearest);
		offsets.WrapS(TextureWrap::ClampToBorder);
		offsets.WrapT(TextureWrap::ClampToBorder);
	}

	void init_heights(void)
	{
		ProgramUniformSampler(display_prog, "Heights").Set(1);
		ProgramUniformSampler(shadow_prog, "Heights").Set(1);
		heights.target = Texture::Target::_2D;
		heights.BindMulti(1, Texture::Target::_2D);
		heights.Image2D(
			0,
			PixelDataInternalFormat::DepthComponent32,
			side, side,
			0,
			PixelDataFormat::DepthComponent,
			PixelDataType::Float,
			nullptr
		);
		heights.BorderColor(Vec4f(1,1,1,1));
		heights.MinFilter(TextureMinFilter::Nearest);
		heights.MagFilter(TextureMagFilter::Nearest);
		heights.WrapS(TextureWrap::ClampToBorder);
		heights.WrapT(TextureWrap::ClampToBorder);

		heights_fbo.target = Framebuffer::Target::Draw;
		heights_fbo.AttachTexture(FramebufferAttachment::Depth, heights, 0);
		heights_fbo.Complete();
	}

	void init_shadows(void)
	{
		ProgramUniformSampler(display_prog, "Shadows").Set(2);
		shadows.target = Texture::Target::_2D;
		shadows.BindMulti(2, Texture::Target::_2D);
		shadows.Image2D(
			0,
			PixelDataInternalFormat::DepthComponent32,
			shadow_size, shadow_size,
			0,
			PixelDataFormat::DepthComponent,
			PixelDataType::Float,
			nullptr
		);
		shadows.BorderColor(Vec4f(1,1,1,1));
		shadows.MinFilter(TextureMinFilter::Linear);
		shadows.MagFilter(TextureMagFilter::Linear);
		shadows.WrapS(TextureWrap::ClampToBorder);
		shadows.WrapT(TextureWrap::ClampToBorder);
		shadows.CompareMode(TextureCompareMode::CompareRefToTexture);

		shadows_fbo.target = Framebuffer::Target::Draw;
		shadows_fbo.AttachTexture(FramebufferAttachment::Depth, shadows, 0);
		shadows_fbo.Complete();
	}
public:
	BarDisplay(void)
	 : gl()
	 , side(128)
	 , shadow_size(512)
	 , cube(
		List("Position")("Normal").Get(),
		shapes::Cube(0.95, 1.0, 0.95, 0.0, 0.5, 0.0)
	), display_vao(cube.VAOForProgram(display_prog))
	 , shadow_vao(cube.VAOForProgram(shadow_prog))
	{
		init_shadows();
		init_heights();
		init_offsets();

		auto light_proj = CamMatrixf::PerspectiveX(
			Degrees(74),
			1.0,
			1, 3*side
		);
		display_prog.light_proj_matrix.Set(light_proj);
		shadow_prog.projection_matrix.Set(light_proj);
		gl.PolygonOffset(4.0, 4.0);
	}

	void Reshape(GLuint vpw, GLuint vph)
	{
		width = vpw;
		height= vph;

		display_prog.projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(65),
				width, height,
				1, 3*side
			)
		);
	}

	double Update(DisplayScene& scene, double time)
	{
		heights_fbo.Bind(FramebufferTarget::Draw);
		gl.Viewport(side, side);
		return scene.Draw(time);
	}

	void Draw(double time, double fade)
	{
		// Shadow map
		shadows_fbo.Bind(FramebufferTarget::Draw);
		gl.Viewport(shadow_size, shadow_size);
		gl.Clear().DepthBuffer();

		auto light = CamMatrixf::Orbiting(
			Vec3f(0, side*0.25f, 0),
			side*1.5f,
			Degrees(-time * 27),
			Degrees(SineWave(time / 19.0)*25  + 45)
		);

		shadow_prog.fade.Set(GLfloat(fade));
		shadow_prog.camera_matrix.Set(light);

		shadow_prog.Use();
		shadow_vao.Bind();
		gl.Enable(Capability::PolygonOffsetFill);
		cube.Draw(side*side);
		gl.Disable(Capability::PolygonOffsetFill);
		gl.Finish();

		// On-screen
		default_fb.Bind(Framebuffer::Target::Draw);
		gl.Viewport(width, height);
		gl.Clear().ColorBuffer().DepthBuffer();

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			side*1.1f,
			Degrees(time * 19),
			Degrees(SineWave(time / 20.0) * 39 + 50)
		);

		display_prog.fade.Set(GLfloat(fade));
		display_prog.light_pos.Set(light.Position());
		display_prog.camera_pos.Set(camera.Position());
		display_prog.light_matrix.Set(light);
		display_prog.camera_matrix.Set(camera);

		display_prog.Use();
		display_vao.Bind();
		cube.Draw(side*side);
	}
};


class CubeExample : public Example
{
private:
	Context gl;

	BarDisplay bar_display;

	DisplayScene scene;
public:
	CubeExample(void)
	{
		scene.AddShape(shapes::Cage());
		scene.AddShape(shapes::WickerTorus(1.0, 0.5, 0.02, 12, 12));
		scene.AddShape(shapes::SpiralSphere());
		scene.AddShape(shapes::TwistedTorus(0.9, 0.5, 0.02, 8, 48, 7));
		scene.AddShape(shapes::Icosahedron());

		gl.ClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint width, GLuint height)
	{
		bar_display.Reshape(width, height);
	}

	void Render(double time)
	{
		double fade = bar_display.Update(scene, time);
		bar_display.Draw(time, fade);
	}

	bool Continue(double time)
	{
		return time < 120.0;
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
