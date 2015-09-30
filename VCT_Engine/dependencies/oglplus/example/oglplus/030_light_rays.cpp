/**
 *  @example oglplus/030_light_rays.cpp
 *  @brief Shows how to render screen-space light rays effect
 *
 *  @oglplus_screenshot{030_light_rays}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{LAMBDAS}
 *  @oglplus_example_uses_gl{GL_VERSION_3_2}
 *  @oglplus_example_uses_model{large_fan}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/obj_mesh.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/opt/resources.hpp>
#include <oglplus/opt/list_init.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

struct MeshInputFile
{
	std::ifstream stream;

	MeshInputFile(void)
	{
		OpenResourceFile(stream, "models", "large_fan", ".obj");
	}
};

class CommonVertexShader : public VertexShader
{
public:
	CommonVertexShader(void)
	{
		Source(
			"#version 140\n"
			"uniform vec3 LightPosition;"
			"uniform mat4 ProjectionMatrix, CameraMatrix, ModelMatrix, LightMatrix;"

			"in vec4 Position;"
			"in vec3 Normal;"

			"out vec3 vertNormal;"
			"out vec3 vertLightDir;"
			"out vec4 vertShadowCoord;"

			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertLightDir = normalize(LightPosition - gl_Position.xyz);"
			"	vertNormal = normalize(mat3(ModelMatrix) * Normal);"
			"	vertShadowCoord = LightMatrix * ModelMatrix * Position;"
			"	gl_Position = ProjectionMatrix * CameraMatrix * gl_Position;"
			"}"
		);
		Compile();
	}
};

class ShadowProgram : public Program
{
private:
	static Program make(const VertexShader& vs)
	{
		Program prog;

		FragmentShader fs(ObjectDesc("Shadow"));
		fs.Source(
			"#version 140\n"
			"void main(void) { }"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs);
		prog.Link().Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Mat4f> projection_matrix, camera_matrix;

	ShadowProgram(const VertexShader& vs)
	 : Program(make(vs))
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	{
		Uniform<Mat4f>(self(), "ModelMatrix").Set(ModelMatrixf());
	}
};

class MaskProgram : public Program
{
private:
	static Program make(const VertexShader& vs)
	{
		Program prog;

		FragmentShader fs(ObjectDesc("Mask"));
		fs.Source(
			"#version 140\n"

			"uniform sampler2DShadow ShadowMap;"

			"in vec3 vertNormal;"
			"in vec3 vertLightDir;"
			"in vec4 vertShadowCoord;"

			"out float fragIntensity;"

			"void main(void)"
			"{"
			"	vec3 ShadowCoord = (vertShadowCoord.xyz/vertShadowCoord.w)*0.5 + 0.5;"
			"	float s = 0.0f;"
			"	if("
			"		ShadowCoord.x >= 0.0 && "
			"		ShadowCoord.x <= 1.0 && "
			"		ShadowCoord.y >= 0.0 && "
			"		ShadowCoord.y <= 1.0 && "
			"		ShadowCoord.z <= 1.0"
			"	) s = max(texture(ShadowMap, ShadowCoord), 0.05);"
			"	float l = max(dot(vertNormal, vertLightDir)+0.1, 0.0);"
			"	fragIntensity = l * s;"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs);
		prog.Link().Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Vec3f> light_position;
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix, light_matrix;

	MaskProgram(const VertexShader& vs)
	 : Program(make(vs))
	 , light_position(self(), "LightPosition")
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , model_matrix(self(), "ModelMatrix")
	 , light_matrix(self(), "LightMatrix")
	{ }
};

class DrawProgram : public Program
{
private:
	static Program make(const VertexShader& vs)
	{
		Program prog;

		FragmentShader fs(ObjectDesc("Draw"));
		fs.Source(
			"#version 140\n"
			"const vec3 LightColor = vec3(0.6, 0.6, 1.0);"
			"const vec3 Up = normalize(vec3(0.1, 1.0, 0.1));"

			"uniform vec3 LightScreenPos;"
			"uniform vec2 ScreenSize;"
			"uniform sampler2DRect LightMap;"
			"uniform sampler2DShadow ShadowMap;"

			"in vec3 vertNormal;"
			"in vec3 vertLightDir;"
			"in vec4 vertShadowCoord;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	vec3 ShadowCoord = (vertShadowCoord.xyz/vertShadowCoord.w)*0.5 + 0.5;"
			"	float s = 0.0f;"
			"	if("
			"		ShadowCoord.x >= 0.0 && "
			"		ShadowCoord.x <= 1.0 && "
			"		ShadowCoord.y >= 0.0 && "
			"		ShadowCoord.y <= 1.0 && "
			"		ShadowCoord.z <= 1.0"
			"	) s = texture(ShadowMap, ShadowCoord);"
			"	float a = 0.1*(max(dot(vertNormal, Up)+0.1, 0.0)+0.1);"
			"	float d = max(dot(vertNormal, vertLightDir)+0.1, 0.0)+a;"


			"	vec2 LMCoord = gl_FragCoord.xy;"
			"	vec2 LPos = (LightScreenPos.xy*0.5+0.5)*ScreenSize;"
			"	vec2 Ray = LMCoord - LPos;"
			"	float Len = length(Ray);"
			"	int NSampl = int(max(abs(Ray.x), abs(Ray.y)))+1;"
			"	vec2 RayStep = Ray / NSampl;"
			"	float r = texture(LightMap, LMCoord).r;"
			"	NSampl = min(NSampl, int(min(ScreenSize.x, ScreenSize.y)*0.25));"
			"	for(int s=0; s!=NSampl;++s)"
			"	{"
			"		r += texture(LightMap, LPos+RayStep*s).r;"
			"	}"
			"	r /= NSampl;"
			"	r = min(r, 1.0);"
			"	fragColor = LightColor * (mix(a, d, s) + r);"
			"}"
		).Compile();

		prog.AttachShader(vs).AttachShader(fs);
		prog.Link().Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	Uniform<Vec3f> light_position;
	Uniform<Vec3f> light_screen_pos;
	Uniform<Vec2f> screen_size;
	Uniform<Mat4f> projection_matrix, camera_matrix, model_matrix, light_matrix;

	DrawProgram(const VertexShader& vs)
	 : Program(make(vs))
	 , light_position(self(), "LightPosition")
	 , light_screen_pos(self(), "LightScreenPos")
	 , screen_size(self(), "ScreenSize")
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , model_matrix(self(), "ModelMatrix")
	 , light_matrix(self(), "LightMatrix")
	{ }
};

class LightRayExample : public Example
{
private:
	Context gl;

	MeshInputFile mesh_input;
	shapes::ObjMesh mesh_loader;
	shapes::ShapeWrapper meshes;
	GLuint fan_index;

	Vec3f light_position;

	CommonVertexShader vert_shader;

	MaskProgram mask_prog;
	VertexArray mask_vao;

	DrawProgram draw_prog;
	VertexArray draw_vao;

	TextureUnitSelector shadow_tex_unit;
	Texture shadow_map;

	void RenderShadowMap(GLuint size)
	{
		// matrices
		auto lt_proj= CamMatrixf::PerspectiveX(Degrees(12), 1.0, 85.0, 110.0);
		auto light = CamMatrixf::LookingAt(light_position, Vec3f());
		// setup the texture
		Texture::Active(shadow_tex_unit);

		mask_prog.Use();
		Uniform<Mat4f>(mask_prog, "LightMatrix").Set(lt_proj*light);
		UniformSampler(mask_prog, "ShadowMap").Set(GLuint(shadow_tex_unit));

		draw_prog.Use();
		Uniform<Mat4f>(draw_prog, "LightMatrix").Set(lt_proj*light);
		UniformSampler(draw_prog, "ShadowMap").Set(GLuint(shadow_tex_unit));

		Texture::Target tex_tgt = Texture::Target::_2D;
		shadow_map.Bind(tex_tgt);
		Texture::MinFilter(tex_tgt, TextureMinFilter::Linear);
		Texture::MagFilter(tex_tgt, TextureMagFilter::Linear);
		Texture::WrapS(tex_tgt, TextureWrap::ClampToEdge);
		Texture::WrapT(tex_tgt, TextureWrap::ClampToEdge);
		Texture::CompareMode(tex_tgt, TextureCompareMode::CompareRefToTexture);
		Texture::Image2D(
			tex_tgt,
			0,
			PixelDataInternalFormat::DepthComponent32,
			size, size,
			0,
			PixelDataFormat::DepthComponent,
			PixelDataType::Float,
			nullptr
		);

		// create shadow program
		ShadowProgram shadow_prog(vert_shader);

		// VAO for the meshes in shadow program
		VertexArray vao = meshes.VAOForProgram(shadow_prog);
		vao.Bind();

		// FBO for offscreen rendering of the shadow map
		Framebuffer::Target fbo_tgt = Framebuffer::Target::Draw;
		Framebuffer fbo;
		fbo.Bind(fbo_tgt);
		Framebuffer::AttachTexture(fbo_tgt, FramebufferAttachment::Depth, shadow_map, 0);

		// RBO for offscreen rendering
		Renderbuffer::Target rbo_tgt = Renderbuffer::Target::Renderbuffer;
		Renderbuffer rbo;
		rbo.Bind(rbo_tgt);
		Renderbuffer::Storage(rbo_tgt, PixelDataInternalFormat::RGBA, size, size);
		Framebuffer::AttachRenderbuffer(fbo_tgt, FramebufferAttachment::Color, rbo);

		// setup the matrices
		shadow_prog.projection_matrix.Set(lt_proj);
		shadow_prog.camera_matrix.Set(light);

		// setup and clear the viewport
		gl.Viewport(size, size);
		gl.Clear().DepthBuffer();

		// draw the meshes
		gl.PolygonOffset(1.0, 1.0);
		gl.Enable(Capability::PolygonOffsetFill);
		meshes.Draw();
		gl.Disable(Capability::PolygonOffsetFill);
		gl.Finish();

		// bind the default framebuffer
		DefaultFramebuffer().Bind(Framebuffer::Target::Draw);
	}

	TextureUnitSelector light_tex_unit;
	Texture light_mask;
	Framebuffer light_fbo;
	Renderbuffer light_rbo;

	void SetupLightMask(void)
	{
		Texture::Active(light_tex_unit);
		Texture::Target tex_tgt = Texture::Target::Rectangle;
		light_mask.Bind(tex_tgt);

		draw_prog.Use();
		UniformSampler(draw_prog, "LightMap").Set(GLuint(light_tex_unit));

		Texture::MinFilter(tex_tgt, TextureMinFilter::Linear);
		Texture::MagFilter(tex_tgt, TextureMagFilter::Linear);
		Texture::WrapS(tex_tgt, TextureWrap::ClampToEdge);
		Texture::WrapT(tex_tgt, TextureWrap::ClampToEdge);

		Framebuffer::Target fbo_tgt = Framebuffer::Target::Draw;
		light_fbo.Bind(fbo_tgt);
		Framebuffer::AttachTexture(fbo_tgt, FramebufferAttachment::Color, light_mask, 0);

		Renderbuffer::Target rbo_tgt = Renderbuffer::Target::Renderbuffer;
		light_rbo.Bind(rbo_tgt);
		Framebuffer::AttachRenderbuffer(fbo_tgt, FramebufferAttachment::Depth, light_rbo);
	}

	void ResizeLightMask(GLuint width, GLuint height)
	{
		Texture::Active(light_tex_unit);
		Texture::Target tex_tgt = Texture::Target::Rectangle;
		light_mask.Bind(tex_tgt);
		Texture::Image2D(
			tex_tgt,
			0,
			PixelDataInternalFormat::Red,
			width, height,
			0,
			PixelDataFormat::Red,
			PixelDataType::UnsignedByte,
			nullptr
		);

		Renderbuffer::Target rbo_tgt = Renderbuffer::Target::Renderbuffer;
		light_rbo.Bind(rbo_tgt);
		Renderbuffer::Storage(
			rbo_tgt,
			PixelDataInternalFormat::DepthComponent,
			width,
			height
		);

	}

	Mat4f projection;
public:
	LightRayExample(void)
	 : gl()
	 , mesh_loader(
		mesh_input.stream,
		shapes::ObjMesh::LoadingOptions(false).Normals()
	), meshes(List("Position")("Normal").Get(), mesh_loader)
	 , fan_index(mesh_loader.GetMeshIndex("Fan"))
	 , light_position(0.0, 0.0, -100.0)
	 , vert_shader()
	 , mask_prog(vert_shader)
	 , mask_vao(meshes.VAOForProgram(mask_prog))
	 , draw_prog(vert_shader)
	 , draw_vao(meshes.VAOForProgram(draw_prog))
	 , shadow_tex_unit(0)
	 , light_tex_unit(1)
	{
		mesh_input.stream.close();

		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);

		RenderShadowMap(512);

		SetupLightMask();

		mask_prog.Use();
		mask_prog.light_position.Set(light_position);

		draw_prog.Use();
		draw_prog.light_position.Set(light_position);

		gl.ClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	}

	void Reshape(GLuint width, GLuint height)
	{
		gl.Viewport(width, height);
		projection =
			CamMatrixf::PerspectiveX(
				Degrees(70),
				width, height,
				1, 200
			);

		mask_prog.Use();
		mask_prog.projection_matrix.Set(projection);

		draw_prog.Use();
		draw_prog.projection_matrix.Set(projection);
		draw_prog.screen_size.Set(width, height);

		ResizeLightMask(width, height);
	}

	void Render(double time)
	{
		auto camera =
			CamMatrixf::Roll(Degrees(SineWave(time / 11.0)*7+SineWave(time/13.0)*5))*
			CamMatrixf::Orbiting(
				Vec3f(),
				40.0f,
				Degrees(SineWave(time / 11.0)*10+CosineWave(time/19.0)*10-90),
				Degrees(SineWave(time / 17.0)*10+SineWave(time/13.0)*10)
			);

		auto mm_identity = ModelMatrixf();
		auto mm_rotation = ModelMatrixf::RotationZ(FullCircles(time / 7.0));

		Uniform<Mat4f>* model_matrix = nullptr;

		GLuint drawing_fan = fan_index;
		auto drawing_driver =
			[
				&model_matrix,
				&mm_identity,
				&mm_rotation,
				&drawing_fan
			](GLuint phase) -> bool
			{
				if(phase == drawing_fan)
					model_matrix->Set(mm_rotation);
				else model_matrix->Set(mm_identity);
				return true;
			};

		// render the light mask
		light_fbo.Bind(Framebuffer::Target::Draw);

		gl.Clear().ColorBuffer().DepthBuffer();

		mask_vao.Bind();
		mask_prog.Use();
		mask_prog.camera_matrix.Set(camera);
		model_matrix = &mask_prog.model_matrix;

		meshes.Draw(drawing_driver);

		// render the final image
		DefaultFramebuffer().Bind(Framebuffer::Target::Draw);

		gl.Clear().ColorBuffer().DepthBuffer();

		draw_vao.Bind();
		draw_prog.Use();
		Vec4f lsp = projection * camera * Vec4f(light_position, 1.0);
		draw_prog.light_screen_pos = lsp.xyz()/lsp.w();
		draw_prog.camera_matrix.Set(camera);
		model_matrix = &draw_prog.model_matrix;

		meshes.Draw(drawing_driver);
	}

	bool Continue(double time)
	{
		return time < 90.0;
	}

	double ScreenshotTime(void) const
	{
		return 17.0;
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
	return std::unique_ptr<Example>(new LightRayExample);
}

} // namespace oglplus
