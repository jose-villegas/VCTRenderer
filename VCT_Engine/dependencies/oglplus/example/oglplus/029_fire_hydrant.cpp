/**
 *  @example oglplus/029_fire_hydrant.cpp
 *  @brief Shows how to use the BlenderMesh shape generator/loader
 *
 *  @oglplus_screenshot{029_fire_hydrant}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/blender_mesh.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/images/png.hpp>
#include <oglplus/images/load.hpp>
#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>
#include <oglplus/bound/renderbuffer.hpp>

#include <oglplus/opt/resources.hpp>
#include <oglplus/opt/list_init.hpp>

#include <cstdlib>
#include <cmath>

#include "example.hpp"

namespace oglplus {

class ShadowProgram
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;
		VertexShader vs(ObjectDesc("Shadow vertex"));
		vs.Source(
			"#version 140\n"
			"uniform mat4 LightProjMatrix,LightMatrix,ModelMatrix;"
			"mat4 Matrix =LightProjMatrix*LightMatrix*ModelMatrix;"

			"in vec4 Position;"

			"void main(void)"
			"{"
			"	gl_Position = Matrix * Position;"
			"}"
		);
		vs.Compile();
		prog.AttachShader(vs);

		FragmentShader fs(ObjectDesc("Shadow fragment"));
		fs.Source(
			"#version 140\n"
			"void main(void) { }"
		);
		fs.Compile();

		prog.AttachShader(fs);
		prog.Link();

		return prog;
	}

	const Program& self(void) const { return *this; }
public:
	ProgramUniform<Mat4f> light_matrix, model_matrix;

	ShadowProgram(void)
	 : Program(make())
	 , light_matrix(self(), "LightMatrix")
	 , model_matrix(self(), "ModelMatrix")
	{ }
};

class DrawProgram
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;
		VertexShader vs(ObjectDesc("Draw vertex"));
		vs.Source(
			"#version 140\n"
			"uniform vec3 LightPosition, CameraPosition;"
			"uniform mat4 ProjectionMatrix,CameraMatrix,ModelMatrix;"
			"mat3 ModelRotMatrix = mat3(ModelMatrix);"
			"uniform mat4 LightProjMatrix,LightMatrix;"
			"mat4 LtMat = LightProjMatrix*LightMatrix*ModelMatrix;"

			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec3 Tangent;"
			"in vec3 Bitangent;"
			"in vec2 TexCoord;"

			"out vec3 vertNormal, vertTangent, vertBitangent;"
			"out vec3 vertLightDir, vertViewDir;"
			"out vec2 vertTexCoord;"
			"out vec4 vertShadowCoord;"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"	vertViewDir = CameraPosition - gl_Position.xyz;"
			"	gl_Position = ProjectionMatrix*CameraMatrix*gl_Position;"

			"	vertNormal = ModelRotMatrix*Normal;"
			"	vertTangent = ModelRotMatrix*Tangent;"
			"	vertBitangent = ModelRotMatrix*Bitangent;"
			"	vertTexCoord = TexCoord;"
			"	vertShadowCoord = LtMat * Position;"
			"}"
		);
		vs.Compile();
		prog.AttachShader(vs);

		FragmentShader fs(ObjectDesc("Draw fragment"));
		fs.Source(
			"#version 140\n"

			"uniform sampler2D NormalMap, LightingMap, ColorMap;"
			"uniform sampler2DShadow ShadowMap;"

			"const int ShadowSamples = 32;"
			"uniform vec3 ShadowOffs[32];"
			"const float InvShadowSamples = 1.0 / ShadowSamples;"

			"const vec3 LightColor = vec3(1.0, 1.0, 1.0);"

			"in vec3 vertNormal, vertTangent, vertBitangent;"
			"in vec3 vertLightDir, vertViewDir;"
			"in vec2 vertTexCoord;"
			"in vec4 vertShadowCoord;"

			"out vec3 fragColor;"
			"void main(void)"
			"{"
			"	vec3 NMap = normalize(texture(NormalMap, vertTexCoord).rgb);"
			"	vec3 NAdj = vec3(NMap.x*2.0-1.0, NMap.y*2.0-1.0, NMap.z);"
			"	vec3 LMap = texture(LightingMap, vertTexCoord).rgb;"
			"	vec3 Color = texture(ColorMap, vertTexCoord).rgb;"

			"	float inv_w = 1.0/vertShadowCoord.w;"
			"	float f = vertShadowCoord.w / 128.0;"
			"	float Shadow = 0.0;"
			"	for(int s=0; s!=ShadowSamples; ++s)"
			"	{"
			"		vec3 SampleCoord = ((ShadowOffs[s]*f+vertShadowCoord.xyz)*inv_w) * 0.5 + 0.5;"

			"		if("
			"			SampleCoord.x >= 0.0 && "
			"			SampleCoord.x <= 1.0 && "
			"			SampleCoord.y >= 0.0 && "
			"			SampleCoord.y <= 1.0 && "
			"			SampleCoord.z <= 1.0"
			"		) Shadow += texture(ShadowMap, SampleCoord)*InvShadowSamples;"
			"		else Shadow += InvShadowSamples;"
			"	}"

			"	vec3 LightDir = normalize(vertLightDir);"
			"	vec3 ViewDir = normalize(vertViewDir);"
			"	vec3 Normal = normalize(vertNormal);"
			"	vec3 Tangent = normalize(vertTangent);"
			"	vec3 Bitangent = normalize(vertBitangent);"
			"	vec3 fragNormal = Tangent*NAdj.x + Bitangent*NAdj.y + Normal*NAdj.z;"
			"	vec3 LightRefl = reflect(-LightDir, fragNormal);"

			"	float Ambi = sqrt(max(dot(fragNormal, vec3(0.0, 1.0, 0.0))+0.2, 0.0))*0.6 + 0.4;"
			"	float Diff = sqrt(max(dot(fragNormal, LightDir), 0.0))*Shadow;"
			"	float Spec = pow(max(dot(LightRefl, ViewDir)+0.15, 0.0), 0.5+1.5*LMap.b)*Shadow;"

			"	fragColor = "
			"			(LMap.r*0.5)*Ambi*Color +"
			"			(LMap.g*0.8)*Diff*Color +"
			"			(LMap.b*0.4)*Spec*LightColor;"
			"}"
		);
		fs.Compile();

		prog.AttachShader(fs);
		prog.Link();
		return prog;
	}

	const Program& self(void) const { return *this; }
public:
	ProgramUniform<Vec3f> light_position, camera_position;
	ProgramUniform<Mat4f> projection_matrix, camera_matrix, light_matrix, model_matrix;

	DrawProgram(void)
	 : Program(make())
	 , light_position(self(), "LightPosition")
	 , camera_position(self(), "CameraPosition")
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , light_matrix(self(), "LightMatrix")
	 , model_matrix(self(), "ModelMatrix")
	{
		ProgramUniform<Vec3f> shadow_offs(*this, "ShadowOffs");
		for(GLuint i=0; i!=32; ++i)
		{
			float u = std::rand() / float(RAND_MAX);
			float v = std::rand() / float(RAND_MAX);
			shadow_offs[i].Set(
				float(std::sqrt(v) * std::cos(2*3.1415*u)),
				float(std::sqrt(v) * std::sin(2*3.1415*u)),
				0.0f
			);
		}
	}
};

class BlenderMeshExample : public Example
{
private:
	Context gl;

	ShadowProgram shadow_prog;
	DrawProgram draw_prog;

	shapes::ShapeWrapper hydrant;

	static shapes::ShapeWrapper load_hydrant(void)
	{
		std::ifstream input;
		OpenResourceFile(input, "models", "hydrant", ".blend");
		if(!input.good())
			throw std::runtime_error("Error opening file for reading");
		imports::BlendFile blend_file(input);

		return shapes::ShapeWrapper(
			List("Position")("Normal")("Tangent")("Bitangent")("TexCoord").Get(),
			shapes::BlenderMesh(blend_file, List("hydrant").Get())
		);
	}

	VertexArray shadow_vao, draw_vao;

	const GLuint ntex;
	Array<Texture> tex;

	GLuint width, height;
	const GLuint smap_side;
	Texture smap;
	Framebuffer smap_fbo;
	Renderbuffer smap_rbo;

	CubicBezierLoop<Vec3f, double> light_path;
	CubicBezierLoop<Vec3f, double> camera_path;
	CubicBezierLoop<Vec3f, double> target_path;
public:
	BlenderMeshExample(void)
	 : shadow_prog()
	 , draw_prog()
	 , hydrant(load_hydrant())
	 , shadow_vao(hydrant.VAOForProgram(shadow_prog))
	 , draw_vao(hydrant.VAOForProgram(draw_prog))
	 , ntex(3)
	 , tex(ntex)
	 , smap_side(1024)
	 , light_path(
		ListOf<Vec3f>
			(Vec3f(-15.0f, 10.0f,-12.5f))
			(Vec3f(-15.0f, 14.0f,  0.5f))
			(Vec3f(-12.0f, 20.0f, 17.0f))
			(Vec3f(  0.0f, 14.0f, 12.0f))
			(Vec3f( 11.0f, 11.0f, 11.5f))
			(Vec3f(  7.2f, 11.0f,-10.0f))
		.Get()
	), camera_path(
		ListOf<Vec3f>
			(Vec3f(  6.2f, 12.0f, -7.0f))
			(Vec3f(  4.0f, 15.0f,  0.0f))
			(Vec3f(  5.0f, 12.0f,  6.5f))
			(Vec3f( -5.0f, 13.0f,  7.0f))
			(Vec3f( -6.0f, 11.0f,  0.0f))
			(Vec3f( -4.0f, 14.0f, -6.5f))
		.Get()
	), target_path(
		ListOf<Vec3f>
			(Vec3f(  3.2f,  2.0f, -0.1f))
			(Vec3f( -3.2f,  3.0f, -0.3f))
			(Vec3f(  0.3f,  1.0f,  1.3f))
		.Get()
	)
	{
		const char* tex_filenames[3] = {"hydrant_normal", "hydrant_light", "hydrant_color"};
		const char* tex_samplers[3] = {"NormalMap", "LightingMap",  "ColorMap"};

		for(GLuint t=0; t!=ntex; ++t)
		{
			std::ifstream input;
			OpenResourceFile(input, "models", tex_filenames[t], ".png");

			ProgramUniformSampler(draw_prog, tex_samplers[t]).Set(t);
			Texture::Active(t);

			gl.Bound(Texture::Target::_2D, tex[t])
				.Image2D(images::PNGImage(input))
				.GenerateMipmap()
				.MinFilter(TextureMinFilter::LinearMipmapLinear)
				.MagFilter(TextureMagFilter::Linear)
				.Wrap(TextureWrap::Repeat);
		}
		{
			Texture::Active(ntex);
			ProgramUniformSampler(draw_prog, "ShadowMap").Set(ntex);

			gl.Bound(Texture::Target::_2D, smap)
				.Filter(TextureFilter::Linear)
				.Wrap(TextureWrap::ClampToEdge)
				.CompareMode(TextureCompareMode::CompareRefToTexture)
				.Image2D(
					0,
					PixelDataInternalFormat::DepthComponent32,
					smap_side, smap_side,
					0,
					PixelDataFormat::DepthComponent,
					PixelDataType::Float,
					nullptr
				);


			gl.Bound(Renderbuffer::Target::Renderbuffer, smap_rbo)
				.Storage(
					PixelDataInternalFormat::RGBA,
					smap_side,
					smap_side
				);

			gl.Bound(Framebuffer::Target::Draw, smap_fbo)
				.AttachTexture(FramebufferAttachment::Depth, smap, 0)
				.AttachRenderbuffer(FramebufferAttachment::Color, smap_rbo);
		}
		//
		auto light_proj_mat = CamMatrixf::PerspectiveX(Degrees(40), 1.0f, 1.0f, 100.0f);
		ProgramUniform<Mat4f>(shadow_prog, "LightProjMatrix").Set(light_proj_mat);
		ProgramUniform<Mat4f>(draw_prog,   "LightProjMatrix").Set(light_proj_mat);
		//

		gl.ClearColor(0.6f, 0.6f, 0.5f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.PolygonOffset(1.0, 1.0);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;
		draw_prog.projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(75),
				width, height,
				1, 40
			)
		);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();


		auto light_pos = light_path.Position(time / 9.0);
		auto light_matrix = CamMatrixf::LookingAt(light_pos, Vec3f(0.0, 2.0, 0.0));

		auto camera_matrix = CamMatrixf::LookingAt(
			camera_path.Position(time / 19.0),
			target_path.Position(time / 11.0)
		);
		auto model_1 =
			ModelMatrixf::Translation( 3.0f, 0.0f, 0.0f)*
			ModelMatrixf::RotationY(Degrees(45));
		auto model_2 =
			ModelMatrixf::Translation(-4.0f, 0.0f, 0.0f)*
			ModelMatrixf::RotationY(Degrees(-175));

		// render the shadow map
		gl.Bind(Framebuffer::Target::Draw, smap_fbo);
		gl.Viewport(smap_side, smap_side);
		gl.Clear().DepthBuffer();

		gl.Enable(Capability::PolygonOffsetFill);

		shadow_prog.light_matrix.Set(light_matrix);

		gl.Use(shadow_prog);
		gl.Bind(shadow_vao);

		shadow_prog.model_matrix.Set(model_1);
		hydrant.Draw();
		shadow_prog.model_matrix.Set(model_2);
		hydrant.Draw();

		// render the frame
		gl.Bind(Framebuffer::Target::Draw, DefaultFramebuffer());
		gl.Viewport(width, height);
		gl.Clear().ColorBuffer().DepthBuffer();

		gl.Disable(Capability::PolygonOffsetFill);

		draw_prog.light_position.Set(light_pos);
		draw_prog.camera_position.Set(camera_matrix.Position());
		draw_prog.camera_matrix.Set(camera_matrix);
		draw_prog.light_matrix.Set(light_matrix);

		gl.Use(draw_prog);
		gl.Bind(draw_vao);

		draw_prog.model_matrix.Set(model_1);
		hydrant.Draw();
		draw_prog.model_matrix.Set(model_2);
		hydrant.Draw();
	}

	bool Continue(double time)
	{
		return time < 90.0;
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
	return std::unique_ptr<Example>(new BlenderMeshExample);
}

} // namespace oglplus
