/**
 *  @example oglplus/033_metal_and_glass.cpp
 *  @brief Shows a little more advanced lighting and blending
 *
 *  @oglplus_screenshot{033_metal_and_glass}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_cpp_feat{LAMBDAS}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects}
 *  @oglplus_example_uses_gl{GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/plane.hpp>
#include <oglplus/shapes/wicker_torus.hpp>

#include <oglplus/images/brushed_metal.hpp>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>
#include <oglplus/bound/renderbuffer.hpp>

#include <cmath>
#include <functional>

#include "example.hpp"

namespace oglplus {

class TransformProgram : public Program
{
private:
	static Program make(void)
	{
		const GLchar* shader_source =
		"#version 150\n"
		"uniform mat4 CameraMatrix, ModelMatrix;"
		"uniform mat4 LightProjMatrix;"
		"uniform mat2 TextureMatrix;"
		"uniform vec3 CameraPosition, LightPosition;"
		"uniform vec4 ClipPlane;"
		"uniform float ClipDirection;"
		"in vec4 Position;"
		"in vec3 Normal;"
		"in vec3 Tangent;"
		"in vec2 TexCoord;"
		"out gl_PerVertex {"
		"	vec4 gl_Position;"
		"	float gl_ClipDistance[];"
		"};"
		"out vec3 vertNormal;"
		"out vec3 vertTangent;"
		"out vec3 vertBitangent;"
		"out vec3 vertLightDir;"
		"out vec3 vertViewDir;"
		"out vec2 vertTexCoord;"
		"out vec4 vertLightTexCoord;"
		"void main(void)"
		"{"
		"	gl_Position = "
		"		ModelMatrix *"
		"		Position;"
		"	gl_ClipDistance[0] = "
		"		ClipDirection* "
		"		dot(ClipPlane, gl_Position);"
		"	vertLightDir = LightPosition - gl_Position.xyz;"
		"	vertViewDir = CameraPosition - gl_Position.xyz;"
		"	vertNormal =  (ModelMatrix * vec4(Normal,  0.0)).xyz;"
		"	vertTangent = (ModelMatrix * vec4(Tangent, 0.0)).xyz;"
		"	vertBitangent = cross(vertNormal, vertTangent);"
		"	vertTexCoord = TextureMatrix * TexCoord;"
		"	vertLightTexCoord = LightProjMatrix* gl_Position;"
		"	gl_Position = CameraMatrix * gl_Position;"
		"}";

		return ShaderProgram(
			ShaderType::Vertex,
			shader_source,
			ObjectDesc("Transform")
		);
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Mat4f> camera_matrix, model_matrix, light_proj_matrix;
	ProgramUniform<Mat2f> texture_matrix;
	ProgramUniform<Vec3f> camera_position, light_position;
	ProgramUniform<Vec4f> clip_plane;
	ProgramUniform<GLfloat> clip_direction;

	TransformProgram(void)
	 : Program(make())
	 , camera_matrix(prog(), "CameraMatrix")
	 , model_matrix(prog(), "ModelMatrix")
	 , light_proj_matrix(prog(), "LightProjMatrix")
	 , texture_matrix(prog(), "TextureMatrix")
	 , camera_position(prog(), "CameraPosition")
	 , light_position(prog(), "LightPosition")
	 , clip_plane(prog(), "ClipPlane")
	 , clip_direction(prog(), "ClipDirection")
	{ }
};

class ShadowFragmentShader
 : public FragmentShader
{
public:
	ShadowFragmentShader(void)
	 : FragmentShader(
		ObjectDesc("Shadow fragment shader"),
		"#version 150\n"
		"in vec3 vertNormal;"
		"in vec3 vertTangent;"
		"in vec3 vertBitangent;"
		"in vec3 vertLightDir;"
		"in vec3 vertViewDir;"
		"in vec2 vertTexCoord;"
		"in vec4 vertLightTexCoord;"

		"void main(void)"
		"{"
		"}"
	)
	{ }
};

class ShadowProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Shadow"));
		prog.AttachShader(ShadowFragmentShader());
		prog.MakeSeparable().Link().Use();
		return prog;
	}
public:
	ShadowProgram(void)
	 : Program(make())
	{ }
};

class LightFragmentShader
 : public FragmentShader
{
public:
	LightFragmentShader(void)
	 : FragmentShader(
		ObjectDesc("Shadow fragment shader"),
		"#version 150\n"
		"uniform vec3 Color;"
		"in vec3 vertNormal;"
		"in vec3 vertTangent;"
		"in vec3 vertBitangent;"
		"in vec3 vertLightDir;"
		"in vec3 vertViewDir;"
		"in vec2 vertTexCoord;"
		"in vec4 vertLightTexCoord;"
		"out vec4 fragColor;"

		"void main(void)"
		"{"
		"	float Opacity = 1.0 - abs(dot("
		"		normalize(vertNormal),"
		"		normalize(vertViewDir)"
		"	));"
		"	fragColor = vec4(Color, 0.4 + sqrt(Opacity)*0.6);"
		"}"
	)
	{ }
};

class LightProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Light"));
		prog.AttachShader(LightFragmentShader());
		prog.MakeSeparable().Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Vec3f> color;

	LightProgram(void)
	 : Program(make())
	 , color(prog(), "Color")
	{ }
};

class GlassFragmentShader
 : public FragmentShader
{
public:
	GlassFragmentShader(void)
	 : FragmentShader(
		ObjectDesc("Metal fragment shader"),
		"#version 150\n"
		"uniform sampler2DShadow FrameShadowTex;"
		"uniform vec3 Color;"
		"in vec3 vertNormal;"
		"in vec3 vertTangent;"
		"in vec3 vertBitangent;"
		"in vec3 vertLightDir;"
		"in vec3 vertViewDir;"
		"in vec2 vertTexCoord;"
		"in vec4 vertLightTexCoord;"
		"out vec4 fragColor;"

		"void main(void)"
		"{"
		"	vec3 LightColor = vec3(1.0, 1.0, 1.0);"

		"	vec3 LightProjCoord = ("
		"		vertLightTexCoord.xyz /"
		"		vertLightTexCoord.w"
		"	) * 0.5 + 0.5;"
		"	if("
		"		LightProjCoord.x >= 0.0 && "
		"		LightProjCoord.x <= 1.0 && "
		"		LightProjCoord.y >= 0.0 && "
		"		LightProjCoord.y <= 1.0 && "
		"		LightProjCoord.z <= 1.0"
		"	)"
		"	{"
		"		float Shadow = 0.0;"
		"		const int sn = 12;"
		"		const float o = 1.0/128.0;"
		"		for(int s=0; s!=sn; ++s)"
		"		{"
		"			float r = float(s)/sn;"
		"			float a = 2.0*3.14151*r;"
		"			Shadow += texture("
		"				FrameShadowTex, "
		"				LightProjCoord+"
		"				vec3(cos(a)*o*r, sin(a)*o*r, 0.0)"
		"			);"
		"		}"
		"		LightColor *= Shadow / sn;"
		"	}"

		"	vec3 LightRefl = reflect("
		"		-normalize(vertLightDir),"
		"		normalize(vertNormal)"
		"	);"

		"	float Specular = pow(max(dot("
		"		normalize(LightRefl),"
		"		normalize(vertViewDir)"
		"	)+0.05, 0.0), 128);"

		"	float Diffuse = pow(pow(dot("
		"		normalize(vertNormal), "
		"		normalize(vertLightDir)"
		"	)+0.3, 2.0), 2.0);"

		"	float Ambient = 0.3;"

		"	fragColor = vec4("
		"		Color * Ambient +"
		"		LightColor * Color * Diffuse + "
		"		LightColor * Specular, "
		"		0.4 + min(Specular, 1.0)*0.3"
		"	);"
		"}"
	)
	{ }
};

class GlassProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Glass"));
		prog.AttachShader(GlassFragmentShader());
		prog.MakeSeparable().Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Vec3f> color;
	ProgramUniformSampler frame_shadow_tex;

	GlassProgram(void)
	 : Program(make())
	 , color(prog(), "Color")
	 , frame_shadow_tex(prog(), "FrameShadowTex")
	{ }
};

class MetalFragmentShader
 : public FragmentShader
{
public:
	MetalFragmentShader(void)
	 : FragmentShader(
		ObjectDesc("Metal fragment shader"),
		"#version 150\n"
		"uniform vec3 Color1, Color2;"
		"uniform sampler2D MetalTex;"
		"uniform sampler2DShadow FrameShadowTex;"
		"uniform sampler2D GlassShadowTex;"
		"uniform int WithGlassShadow;"
		"in vec3 vertNormal;"
		"in vec3 vertTangent;"
		"in vec3 vertBitangent;"
		"in vec3 vertLightDir;"
		"in vec3 vertViewDir;"
		"in vec2 vertTexCoord;"
		"in vec4 vertLightTexCoord;"
		"out vec4 fragColor;"

		"void main(void)"
		"{"
		"	vec3 Sample = texture(MetalTex, vertTexCoord).rgb;"
		"	vec3 LightColor = vec3(1.0, 1.0, 1.0);"

		"	vec3 LightProjCoord = ("
		"		vertLightTexCoord.xyz /"
		"		vertLightTexCoord.w"
		"	) * 0.5 + 0.5;"
		"	if("
		"		LightProjCoord.x >= 0.0 && "
		"		LightProjCoord.x <= 1.0 && "
		"		LightProjCoord.y >= 0.0 && "
		"		LightProjCoord.y <= 1.0 && "
		"		LightProjCoord.z <= 1.0"
		"	)"
		"	{"
		"		float Shadow = 0.0;"
		"		const int sn = 12;"
		"		const float o = 1.0/128.0;"
		"		for(int s=0; s!=sn; ++s)"
		"		{"
		"			float r = float(s)/sn;"
		"			float a = 4.0*3.14151*r;"
		"			Shadow += texture("
		"				FrameShadowTex, "
		"				LightProjCoord+"
		"				vec3(cos(a)*o*r, sin(a)*o*r, 0.0)"
		"			);"
		"		}"
		"		LightColor *= (Shadow / sn);"
		"		if(WithGlassShadow != 0)"
		"		{"
		"			vec4 FilterColor = texture("
		"				GlassShadowTex,"
		"				LightProjCoord.st"
		"			);"
		"			LightColor = mix("
		"				LightColor,"
		"				LightColor*FilterColor.rgb,"
		"				FilterColor.a"
		"			);"
		"		}"
		"	}"

		"	vec3 Normal = normalize("
		"		2.0*vertNormal + "
		"		(Sample.r - 0.5)*vertTangent + "
		"		(Sample.g - 0.5)*vertBitangent"
		"	);"

		"	vec3 LightRefl = reflect("
		"		-normalize(vertLightDir),"
		"		Normal"
		"	);"

		"	float Specular = pow(max(dot("
		"		normalize(LightRefl),"
		"		normalize(vertViewDir)"
		"	)+0.02, 0.0), 16+Sample.b*48)*pow(0.4+Sample.b*1.6, 4.0);"

		"	Normal = normalize(vertNormal*3.0 + Normal);"

		"	float Diffuse = pow(max(dot("
		"		normalize(Normal), "
		"		normalize(vertLightDir)"
		"	), 0.0), 2.0);"

		"	float Ambient = (dot("
		"		normalize(vertNormal),"
		"		vec3(0.0, 1.0, 0.0)"
		"	)*0.25 + 0.75) * 0.3;"

		"	vec3 Color = mix(Color1, Color2, Sample.b);"

		"	fragColor = vec4("
		"		Color * Ambient +"
		"		LightColor * Color * Diffuse + "
		"		LightColor * Specular, "
		"		1.0"
		"	);"
		"}"
	)
	{ }
};

class MetalProgram : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Metal"));
		prog.AttachShader(MetalFragmentShader());
		prog.MakeSeparable().Link().Use();
		return prog;
	}
	const Program& prog(void) const { return *this; }
public:
	ProgramUniform<Vec3f> color_1, color_2;
	ProgramUniformSampler metal_tex, frame_shadow_tex, glass_shadow_tex;
	ProgramUniform<GLint> with_glass_shadow;

	MetalProgram(void)
	 : Program(make())
	 , color_1(prog(), "Color1")
	 , color_2(prog(), "Color2")
	 , metal_tex(prog(), "MetalTex")
	 , frame_shadow_tex(prog(), "FrameShadowTex")
	 , glass_shadow_tex(prog(), "GlassShadowTex")
	 , with_glass_shadow(prog(), "WithGlassShadow")
	{ }
};


template <typename ShapeBuilder>
class Shape
{
protected:
	// helper object building shape vertex attributes
	ShapeBuilder make_shape;
	// helper object encapsulating shape drawing instructions
	shapes::DrawingInstructions shape_instr;
	// indices pointing to shape primitive elements
	typename ShapeBuilder::IndexArray shape_indices;

	Context gl;

	// A vertex array object for the rendered shape
	VertexArray vao;

	// VBOs for the shape's vertex attributes
	Array<Buffer> vbos;

public:
	Shape(const Program& prog, const ShapeBuilder& builder)
	 : make_shape(builder)
	 , shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , vbos(4)
	{
		// bind the VAO for the shape
		vao.Bind();

		typename ShapeBuilder::VertexAttribs vert_attr_info;
		const GLuint nva = 4;
		const GLchar* vert_attr_name[nva] = {
			"Position",
			"Normal",
			"Tangent",
			"TexCoord"
		};
		for(GLuint va=0; va!=nva; ++va)
		{
			const GLchar* name = vert_attr_name[va];
			std::vector<GLfloat> data;
			auto getter = vert_attr_info.VertexAttribGetter(data, name);
			if(getter != nullptr)
			{
				// bind the VBO for the vertex attribute
				vbos[va].Bind(Buffer::Target::Array);
				GLuint n_per_vertex = getter(make_shape, data);
				// upload the data
				Buffer::Data(Buffer::Target::Array, data);
				// setup the vertex attribs array
				VertexArrayAttrib attr(prog, name);
				attr.Setup<GLfloat>(n_per_vertex);
				attr.Enable();
			}
		}
	}

	void Draw(const std::function<bool (GLuint)>& drawing_driver)
	{
		vao.Bind();
		gl.FrontFace(make_shape.FaceWinding());
		shape_instr.Draw(shape_indices, 1, 0, drawing_driver);
	}
};

class GlassAndMetalExample : public Example
{
private:

	// wrapper around the current OpenGL context
	Context gl;

	TransformProgram transf_prog;
	ShadowProgram shadow_prog;
	LightProgram light_prog;
	GlassProgram glass_prog;
	MetalProgram metal_prog;

	ProgramPipeline shadow_pp, light_pp, glass_pp, metal_pp;

	Shape<shapes::Plane> plane;
	Shape<shapes::WickerTorus> torus;

	// The burshed metal texture
	Texture metal_texture;

	GLuint width, height;
	const GLuint shadow_tex_side;

	// The metal frame shadow texture and its framebuffer
	Texture frame_shadow_tex;
	Framebuffer frame_shadow_fbo;

	// The glass light filter texture and its framebuffer
	Texture glass_shadow_tex;
	Framebuffer glass_shadow_fbo;

public:
	GlassAndMetalExample(void)
	 : transf_prog()
	 , metal_prog()
	 , plane(transf_prog, shapes::Plane(Vec3f(9, 0, 0), Vec3f(0, 0,-9)))
	 , torus(transf_prog, shapes::WickerTorus())
	 , shadow_tex_side(1024)
	{
		NoProgram().Use();

		shadow_pp.Bind();
		shadow_pp.UseStages(transf_prog).Vertex();
		shadow_pp.UseStages(shadow_prog).Fragment();

		light_pp.Bind();
		light_pp.UseStages(transf_prog).Vertex();
		light_pp.UseStages(light_prog).Fragment();

		glass_pp.Bind();
		glass_pp.UseStages(transf_prog).Vertex();
		glass_pp.UseStages(glass_prog).Fragment();

		metal_pp.Bind();
		metal_pp.UseStages(transf_prog).Vertex();
		metal_pp.UseStages(metal_prog).Fragment();

		Texture::Active(0);
		metal_prog.metal_tex.Set(0);
		gl.Bound(Texture::Target::_2D, metal_texture)
			.MinFilter(TextureMinFilter::LinearMipmapLinear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::Repeat)
			.WrapT(TextureWrap::Repeat)
			.Image2D(
				images::BrushedMetalUByte(
					512, 512,
					5120,
					-3, +3,
					32, 128
				)
			).GenerateMipmap();

		Texture::Active(1);
		metal_prog.frame_shadow_tex.Set(1);
		glass_prog.frame_shadow_tex.Set(1);

		gl.Bound(Texture::Target::_2D, frame_shadow_tex)
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::ClampToEdge)
			.WrapT(TextureWrap::ClampToEdge)
			.CompareMode(TextureCompareMode::CompareRefToTexture)
			.Image2D(
				0,
				PixelDataInternalFormat::DepthComponent32,
				shadow_tex_side, shadow_tex_side,
				0,
				PixelDataFormat::DepthComponent,
				PixelDataType::Float,
				nullptr
			);

		gl.Bound(Framebuffer::Target::Draw, frame_shadow_fbo)
			.AttachTexture(
				FramebufferAttachment::Depth,
				frame_shadow_tex,
				0
			);

		Texture::Active(2);
		metal_prog.glass_shadow_tex.Set(2);

		gl.Bound(Texture::Target::_2D, glass_shadow_tex)
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::ClampToEdge)
			.WrapT(TextureWrap::ClampToEdge)
			.Image2D(
				0,
				PixelDataInternalFormat::RGBA,
				shadow_tex_side, shadow_tex_side,
				0,
				PixelDataFormat::RGBA,
				PixelDataType::UnsignedByte,
				nullptr
			);

		gl.Bound(Framebuffer::Target::Draw, glass_shadow_fbo)
			.AttachTexture(
				FramebufferAttachment::Color,
				glass_shadow_tex,
				0
			);

		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);

		gl.BlendFunc(BlendFn::SrcAlpha, BlendFn::OneMinusSrcAlpha);

		gl.PolygonOffset(1.0, 1.0);

	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;
	}


	void RenderGlassShadowMap(
		const Vec3f& light_position,
		const Vec3f& torus_center,
		const Mat4f& torus_matrix,
		const Mat4f& light_proj_matrix
	)
	{
		glass_shadow_fbo.Bind(Framebuffer::Target::Draw);

		gl.Viewport(shadow_tex_side, shadow_tex_side);
		const GLfloat clear_color[4] = {1.0f, 1.0f, 1.0f, 0.0f};
		gl.ClearColorBuffer(0, clear_color);

		transf_prog.camera_matrix.Set(light_proj_matrix);
		transf_prog.camera_position.Set(light_position);
		transf_prog.light_proj_matrix.Set(light_proj_matrix);
		transf_prog.light_position.Set(light_position);

		// Render the torus' frame
		transf_prog.model_matrix.Set(torus_matrix);

		// setup the view clipping plane
		Planef clip_plane = Planef::FromPointAndNormal(
			torus_center,
			Normalized(light_position-torus_center)
		);
		transf_prog.clip_plane.Set(clip_plane.Equation());

		light_pp.Bind();

		light_prog.color = Vec3f(0.6f, 0.4f, 0.1f);

		gl.Disable(Capability::DepthTest);
		gl.Enable(Functionality::ClipDistance, 0);
		gl.Enable(Capability::Blend);

		for(int c=0; c!=2; ++c)
		{
			transf_prog.clip_direction.Set((c == 0)?1:-1);

			for(int p=3; p>=0; --p)
			{
				if(p % 2 == 0) gl.CullFace(Face::Front);
				else gl.CullFace(Face::Back);
				torus.Draw(
					[&p](GLuint phase) -> bool
					{
						if(p == 0 || p == 3)
						{
							return (phase == 4);
						}
						else return (phase > 4);
					}
				);
			}
		}
		gl.Disable(Capability::Blend);
		gl.Disable(Functionality::ClipDistance, 0);
		gl.Enable(Capability::DepthTest);
	}

	void RenderFrameShadowMap(
		const Vec3f& light_position,
		const Mat4f& torus_matrix,
		const Mat4f& light_proj_matrix
	)
	{
		frame_shadow_fbo.Bind(Framebuffer::Target::Draw);

		gl.Viewport(shadow_tex_side, shadow_tex_side);
		gl.ClearDepthBuffer(1.0f);
		gl.CullFace(Face::Back);

		transf_prog.camera_matrix.Set(light_proj_matrix);
		transf_prog.camera_position.Set(light_position);

		// Render the torus' frame
		transf_prog.model_matrix.Set(torus_matrix);

		shadow_pp.Bind();

		gl.Enable(Capability::PolygonOffsetFill);
		torus.Draw(
			[](GLuint phase) -> bool
			{
				return (phase <= 3);
			}
		);
		gl.Disable(Capability::PolygonOffsetFill);
	}

	void RenderImage(
		double time,
		const Vec3f& torus_center,
		const Mat4f& torus_matrix,
		const Mat4f& light_proj_matrix
	)
	{
		// this is going into the on-screen framebuffer
		DefaultFramebuffer().Bind(Framebuffer::Target::Draw);

		gl.ClearColor(0.6f, 0.6f, 0.5f, 0.0f);
		gl.Viewport(width, height);
		gl.Clear().ColorBuffer().DepthBuffer();
		gl.CullFace(Face::Back);
		//
		transf_prog.light_proj_matrix.Set(light_proj_matrix);

		Mat4f perspective = CamMatrixf::PerspectiveX(
			Degrees(60),
			float(width)/height,
			1, 60
		);

		// setup the camera
		Vec3f camera_target(0.0f, 0.8f, 0.0f);
		auto camera = CamMatrixf::Orbiting(
			camera_target,
			GLfloat(8.0 - SineWave(time / 15.0)*3.0),
			FullCircles(time / 24.0),
			Degrees(45 + SineWave(time / 20.0) * 40)
		);
		Vec3f camera_position = camera.Position();
		transf_prog.camera_matrix.Set(perspective*camera);
		transf_prog.camera_position.Set(camera_position);

		// setup the view clipping plane
		Planef clip_plane = Planef::FromPointAndNormal(
			torus_center,
			Normalized(camera_position-torus_center)
		);

		metal_pp.Bind();

		// Render the plane
		transf_prog.model_matrix = ModelMatrixf();
		transf_prog.texture_matrix.Set(Mat2f(Vec2f(9.0f,0.0f), Vec2f(0.0f,9.0f)));
		metal_prog.color_1 = Vec3f(1.0f, 0.9f, 0.8f);
		metal_prog.color_2 = Vec3f(0.9f, 0.8f, 0.6f);
		metal_prog.with_glass_shadow = 1;

		plane.Draw([](GLuint) -> bool {return true;});

		// Render the torus
		transf_prog.model_matrix.Set(torus_matrix);
		transf_prog.texture_matrix.Set(Mat2f(Vec2f(16.0f,0.0f), Vec2f(0.0f, 4.0f)));
		metal_prog.metal_tex.Set(0);
		metal_prog.color_1 = Vec3f(0.9f, 0.9f, 0.9f);
		metal_prog.color_2 = Vec3f(0.3f, 0.3f, 0.3f);
		metal_prog.with_glass_shadow = 0;

		// the metal-part
		torus.Draw(
			[](GLuint phase) -> bool
			{
				return (phase <= 3);
			}
		);

		// now the glass part
		glass_pp.Bind();

		glass_prog.color = Vec3f(0.6f, 0.4f, 0.1f);

		gl.Enable(Functionality::ClipDistance, 0);
		gl.Enable(Capability::Blend);
		transf_prog.clip_plane.Set(clip_plane.Equation());

		for(int c=0; c!=2; ++c)
		{
			transf_prog.clip_direction.Set((c == 0)?-1:1);

			for(int p=0; p!=4; ++p)
			{
				if(p % 2 == 0) gl.CullFace(Face::Front);
				else gl.CullFace(Face::Back);
				torus.Draw(
					[&p](GLuint phase) -> bool
					{
						if(p == 0 || p == 3)
							return (phase == 4);
						else return (phase > 4);
					}
				);
			}
		}
		gl.Disable(Capability::Blend);
		gl.Disable(Functionality::ClipDistance, 0);
	}

	void Render(double time)
	{
		const Vec3f light_position(16.0, 10.0, 9.0);
		const Vec3f torus_center(0.0, 1.5, 0.0);

		const Mat4f torus_matrix =
			ModelMatrixf::Translation(torus_center) *
			ModelMatrixf::RotationZ(FullCircles(time / 16.0));

		const Mat4f light_proj_matrix =
			CamMatrixf::PerspectiveX(Degrees(10), 1.0, 1, 80) *
			CamMatrixf::LookingAt(light_position, torus_center);

		transf_prog.light_position.Set(light_position);

		RenderFrameShadowMap(
			light_position,
			torus_matrix,
			light_proj_matrix
		);
		RenderGlassShadowMap(
			light_position,
			torus_center,
			torus_matrix,
			light_proj_matrix
		);
		RenderImage(
			time,
			torus_center,
			torus_matrix,
			light_proj_matrix
		);
	}

	bool Continue(double time)
	{
		return time < 90.0;
	}

	double ScreenshotTime(void) const
	{
		return 7.0;
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
	return std::unique_ptr<Example>(new GlassAndMetalExample);
}

} // namespace oglplus
