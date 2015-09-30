/**
 *  @example oglplus/032_transitions.cpp
 *  @brief Shows how to render various transitions between two frames
 *
 *  @oglplus_screenshot{032_transitions}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_shader_subroutine}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/wicker_torus.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>

#include <oglplus/images/brushed_metal.hpp>
#include <oglplus/images/random.hpp>
#include <oglplus/images/newton.hpp>

#include "example.hpp"

#include <cstdlib>
#include <ctime>

namespace oglplus {

class DrawProgram
 : public Program
{
private:
	static Program make(void)
	{
		Program prog(ObjectDesc("Draw program"));

		prog.AttachShader(VertexShader(
			ObjectDesc("Draw vertex"),
			StrCRef("#version 150\n"
			"uniform vec3 LightPosition;"
			"uniform mat4 ModelMatrix;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec3 Tangent;"
			"in vec2 TexCoord;"

			"out vec3 vertNormal;"
			"out vec3 vertTangent;"
			"out vec3 vertBitangent;"
			"out vec3 vertLightDir;"
			"out vec2 vertTexCoord;"

			"void main(void)"
			"{"
			"	gl_Position = "
			"		ModelMatrix *"
			"		Position;"
			"	vertLightDir = LightPosition - gl_Position.xyz;"
			"	vertNormal = mat3(ModelMatrix)*Normal;"
			"	vertTangent = mat3(ModelMatrix)*Tangent;"
			"	vertBitangent = cross(vertNormal, vertTangent);"
			"	vertTexCoord = TexCoord;"
			"}")
		));

		prog.AttachShader(GeometryShader(
			ObjectDesc("Draw geometry"),
			StrCRef("#version 150\n"
			"layout(triangles) in;"
			"layout(triangle_strip, max_vertices = 6) out;"

			"uniform mat4 CameraMatrix[2];"
			"uniform vec3 CameraPosition[2];"
			"uniform vec3 Color[2];"

			"in vec3 vertNormal[];"
			"in vec3 vertTangent[];"
			"in vec3 vertBitangent[];"
			"in vec3 vertLightDir[];"
			"in vec2 vertTexCoord[];"

			"out vec3 geomNormal;"
			"out vec3 geomTangent;"
			"out vec3 geomBitangent;"
			"out vec3 geomLightDir;"
			"out vec3 geomViewDir;"
			"out vec3 geomColor;"
			"out vec2 geomTexCoord;"

			"void main(void)"
			"{"
			"	for(int l=0; l!=2; ++l)"
			"	{"
			"		gl_Layer = l;"
			"		geomColor = Color[l];"
			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			geomNormal = vertNormal[v];"
			"			geomTangent = vertTangent[v];"
			"			geomBitangent = vertBitangent[v];"
			"			geomTexCoord = vertTexCoord[v];"
			"			geomLightDir = vertLightDir[v];"
			"			geomViewDir = "
			"				CameraPosition[l] - "
			"				gl_in[v].gl_Position.xyz;"
			"			gl_Position = "
			"				CameraMatrix[l] *"
			"				gl_in[v].gl_Position;"
			"			EmitVertex();"
			"		}"
			"		EndPrimitive();"
			"	}"
			"}")
		));

		prog.AttachShader(FragmentShader(
			ObjectDesc("Draw fragment"),
			StrCRef("#version 150\n"
			"uniform sampler2D MetalTexture;"
			"in vec3 geomNormal;"
			"in vec3 geomTangent;"
			"in vec3 geomBitangent;"
			"in vec3 geomLightDir;"
			"in vec3 geomViewDir;"
			"in vec3 geomColor;"
			"in vec2 geomTexCoord;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	vec2 TexCoord = vec2(geomTexCoord.s*16, geomTexCoord.t*4);"
			"	vec3 t = texture(MetalTexture, TexCoord).rgb;"

			"	vec3 Normal = normalize("
			"		2*geomNormal + "
			"		(t.r - 0.5)*geomTangent + "
			"		(t.g - 0.5)*geomBitangent"
			"	);"

			"	vec3 LightRefl = reflect("
			"		-normalize(geomLightDir),"
			"		Normal"
			"	);"

			"	float Diffuse = max(dot("
			"		normalize(Normal), "
			"		normalize(geomLightDir)"
			"	), 0.0);"

			"	float Specular = pow(max(dot("
			"		normalize(LightRefl),"
			"		normalize(geomViewDir)"
			"	)+0.1, 0.0), 8+t.b*24);"

			"	vec3 LightColor = vec3(1.0, 1.0, 1.0);"
			"	vec3 ShapeColor = mix(geomColor, LightColor, t.b);"
			"	fragColor = "
			"		ShapeColor*(Diffuse+0.4) + "
			"		LightColor*Specular*pow(0.6+t.b*0.8, 4.0);"
			"}")
		));

		prog.Link();

		return prog;
	}

	const Program& self(void) const { return *this; }
public:
	ProgramUniform<Mat4f> camera_matrix_0, camera_matrix_1, model_matrix;

	ProgramUniform<Vec3f> camera_position_0, camera_position_1;

	DrawProgram(void)
	 : Program(make())
	 , camera_matrix_0(self(), "CameraMatrix[0]")
	 , camera_matrix_1(self(), "CameraMatrix[1]")
	 , model_matrix(self(), "ModelMatrix")
	 , camera_position_0(self(), "CameraPosition[0]")
	 , camera_position_1(self(), "CameraPosition[1]")
	{ }
};

class MetalTexture
 : public Texture
{
public:
	MetalTexture(TextureUnitSelector unit)
	{
		Texture::Active(unit);
		oglplus::Context::Bound<Texture>(Texture::Target::_2D, *this)
			.Image2D(images::BrushedMetalUByte(512, 512, 5120, -32, 32, 16, 32))
			.GenerateMipmap()
			.MinFilter(TextureMinFilter::LinearMipmapLinear)
			.MagFilter(TextureMagFilter::Linear)
			.Wrap(TextureWrap::Repeat);
	}
};

class NoiseTexture
 : public Texture
{
public:
	NoiseTexture(TextureUnitSelector unit)
	{
		Texture::Active(unit);
		oglplus::Context::Bound<Texture>(Texture::Target::_2D, *this)
			.Image2D(images::RandomRedUByte(512, 512))
			.Filter(TextureFilter::Linear)
			.Wrap(TextureWrap::Repeat);
	}
};

class DrawFramebuffer
 : public Framebuffer
{
private:
	Texture color_tex, depth_tex;
public:
	DrawFramebuffer(TextureUnitSelector color_unit, TextureUnitSelector depth_unit)
	 : Framebuffer(ObjectDesc("Draw FBO"))
	{
		Texture::Active(depth_unit);
		oglplus::Context::Bound(Texture::Target::_2DArray, depth_tex)
			.Filter(TextureFilter::Linear)
			.Wrap(TextureWrap::ClampToEdge);

		Texture::Active(color_unit);
		oglplus::Context::Bound(Texture::Target::_2DArray, color_tex)
			.Filter(TextureFilter::Linear)
			.Wrap(TextureWrap::ClampToEdge);

		Resize(256, 256);
	}

	void Resize(GLuint width, GLuint height)
	{
		if(width < height) width = height;
		if(height < width) height = width;

		oglplus::Context::Bound(Texture::Target::_2DArray, depth_tex).Image3D(
			0,
			PixelDataInternalFormat::DepthComponent,
			width, height, 2,
			0,
			PixelDataFormat::DepthComponent,
			PixelDataType::Float,
			nullptr
		);
		oglplus::Context::Bound(Texture::Target::_2DArray, color_tex).Image3D(
			0,
			PixelDataInternalFormat::RGB,
			width, height, 2,
			0,
			PixelDataFormat::RGB,
			PixelDataType::UnsignedByte,
			nullptr
		);

		oglplus::Context::Bound<Framebuffer>(Framebuffer::Target::Draw, *this)
			.AttachTexture(FramebufferAttachment::Color, color_tex, 0)
			.AttachTexture( FramebufferAttachment::Depth, depth_tex, 0)
			.Complete();
	}
};

class ClearProgram
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		prog.AttachShader(VertexShader(
			ObjectDesc("Clear vertex"),
			StrCRef("#version 150\n"
			"in vec4 Position;"
			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"}")
		));

		prog.AttachShader(GeometryShader(
			ObjectDesc("Clear geometry"),
			StrCRef("#version 150\n"
			"layout(triangles) in;"
			"layout(triangle_strip, max_vertices = 6) out;"

			"uniform vec3 Color1[2], Color2[2];"
			"uniform vec2 Origin[2];"

			"out vec3 geomColor1, geomColor2;"
			"out vec2 geomPosition, geomOrigin;"
			"flat out int geomLayer;"

			"void main(void)"
			"{"
			"	for(gl_Layer=0; gl_Layer!=2; ++gl_Layer)"
			"	{"
			"		geomColor1 = Color1[gl_Layer];"
			"		geomColor2 = Color2[gl_Layer];"
			"		geomOrigin = Origin[gl_Layer];"
			"		geomLayer = gl_Layer;"
			"		for(int v=0; v!=3; ++v)"
			"		{"
			"			gl_Position = gl_in[v].gl_Position;"
			"			geomPosition = gl_Position.xy;"
			"			EmitVertex();"
			"		}"
			"		EndPrimitive();"
			"	}"
			"}")
		));

		prog.AttachShader(FragmentShader(
			ObjectDesc("Clear fragment"),
			StrCRef("#version 150\n"

			"in vec3 geomColor1, geomColor2;"
			"in vec2 geomPosition, geomOrigin;"
			"flat in int geomLayer;"
			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	vec2 v = geomPosition - geomOrigin;"
			"	float l = length(v);"
			"	float a = atan(v.y, v.x)/3.1415;"
			"	float d = 32-geomLayer*15;"
			"	if(l < 0.01)"
			"		fragColor = geomColor1;"
			"	else if(int(d*(1.0 + a)) % 2 == 0)"
			"		fragColor = geomColor1 * 0.9 + l*0.1;"
			"	else"
			"		fragColor = geomColor2 * 0.8 + pow(l,1.4)*0.2;"
			"}")
		));

		prog.Link();

		return prog;
	}

	const Program& self(void) const { return *this; }
public:
	ProgramUniform<Vec2f> origin_0, origin_1;

	ClearProgram(void)
	 : Program(make())
	 , origin_0(self(), "Origin[0]")
	 , origin_1(self(), "Origin[1]")
	{ }
};

class TransitionProgram
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;

		prog.AttachShader(VertexShader(
			ObjectDesc("Transition vertex"),
			StrCRef("#version 400\n"
			"in vec4 Position;"
			"out vec2 vertPosition, vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"	vertPosition = Position.xy;"
			"	vertTexCoord = vec2("
			"		(Position.x*0.5 + 0.5),"
			"		(Position.y*0.5 + 0.5) "
			"	);"
			"}")
		));

		prog.AttachShader(FragmentShader(
			ObjectDesc("Transition fragment"),
			StrCRef("#version 400\n"

			"uniform sampler2DArray FrameTex;"
			"uniform sampler2D NoiseTex;"
			"uniform float MixFactor;"
			"uniform bool Direction;"

			"subroutine vec4 TransitionFunc(void);"
			"subroutine uniform TransitionFunc Transition;"

			"in vec2 vertPosition, vertTexCoord;"
			"out vec4 fragColor;"

			"int Frame0(void) { return Direction?1:0; }"
			"int Frame1(void) { return Direction?0:1; }"

			"float Factor(void) { return Direction?1.0-MixFactor:MixFactor; }"

			"vec4 Texel0(void)"
			"{"
			"	return texelFetch(FrameTex, ivec3(gl_FragCoord.xy, Frame0()), 0);"
			"}"

			"vec4 Texel1(void)"
			"{"
			"	return texelFetch(FrameTex, ivec3(gl_FragCoord.xy, Frame1()), 0);"
			"}"

			"vec4 MixTexels(float factor)"
			"{"
			"	return mix(Texel0(), Texel1(), factor);"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 Vertical(void)"
			"{"
			"	return MixTexels((vertTexCoord.y > Factor())?0.0:1.0);"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 Horizontal(void)"
			"{"
			"	return MixTexels((vertTexCoord.x > Factor())?0.0:1.0);"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 Spiral(void)"
			"{"
			"	vec2 v = vertPosition.xy*31.0;"
			"	float l = sqrt(length(v));"
			"	float t = atan(v.y, v.x)/3.1415;"
			"	return MixTexels((fract(l+t) > Factor())?0.0:1.0);"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 Rings(void)"
			"{"
			"	vec2 v = vertPosition.xy*12.0;"
			"	float l = length(v);"
			"	return MixTexels((fract(l) > Factor())?0.0:1.0);"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 SweepFade(void)"
			"{"
			"	vec2 v = vertTexCoord;"
			"	float l = length(v);"
			"	float a = atan(v.y, v.x)/3.1415;"
			"	return MixTexels(clamp(2*Factor() - a, 0.0, 1.0));"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 CenterFan(void)"
			"{"
			"	vec2 v = vertPosition;"
			"	float l = length(v);"
			"	float a = atan(-v.y, v.x)/3.1415;"
			"	return MixTexels((fract(9*a) > Factor())?0.0:1.0);"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 CornerFan(void)"
			"{"
			"	vec2 v = vertTexCoord;"
			"	float l = length(v);"
			"	float a = atan(-v.y, v.x)/3.1415;"
			"	return MixTexels((fract(15*a) > Factor())?0.0:1.0);"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 Rectangle(void)"
			"{"
			"	float x = (abs(vertPosition.x)<Factor())?1.0:0.0;"
			"	float y = (abs(vertPosition.y)<Factor())?1.0:0.0;"
			"	return MixTexels(x*y);"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 EllipseFade(void)"
			"{"
			"	float t = length(vertPosition)/sqrt(2.0);"
			"	return MixTexels(clamp(2*Factor()-t, 0.0, 1.0));"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 Ellipse(void)"
			"{"
			"	float t = length(vertPosition)/sqrt(2.0);"
			"	return MixTexels((Factor()>t)?1.0:0.0);"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 Circles(void)"
			"{"
			"	int a = 16;"
			"	int s = 2*a;"
			"	float r = sqrt(2.0)*a;"
			"	vec2 fc = gl_FragCoord.xy;"
			"	float cv = length(vec2(int(fc.x) % s - a, int(fc.y) % s - a)/r);"
			"	float tc = vertTexCoord.x * vertTexCoord.y;"
			"	return MixTexels(clamp(3*Factor()-tc-cv, 0.0, 1.0));"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 Blend(void)"
			"{"
			"	return MixTexels(Factor());"
			"}"

			"subroutine(TransitionFunc)"
			"vec4 DiagDissolve(void)"
			"{"
			"	float t = texture(NoiseTex, vertTexCoord).r;"
			"	float tc = vertTexCoord.x*vertTexCoord.y;"
			"	return MixTexels(clamp(3*Factor()-tc-t, 0.0, 1.0));"
			"}"

			"void main(void)"
			"{"
			"	fragColor = Transition();"
			"}")
		));

		prog.Link();

		return prog;
	}

	const Program& self(void) const { return *this; }

	UniformSubroutines frag_subroutines;
	SubroutineUniform frag_transition;
	std::vector<Subroutine> transitions;

	GLfloat prev_mix_factor;
public:
	ProgramUniform<GLfloat> mix_factor;
	ProgramUniform<GLint> direction;

	TransitionProgram(void)
	 : Program(make())
	 , frag_subroutines(self(), ShaderType::Fragment)
	 , frag_transition(self(), ShaderType::Fragment, "Transition")
	 , mix_factor(self(), "MixFactor")
	 , direction(self(), "Direction")
	{
		Use();

		ShaderType stage = ShaderType::Fragment;
		for(auto sr=ActiveSubroutines(stage); !sr.Empty(); sr.Next())
			transitions.push_back(Subroutine(self(), stage, sr.Front().Name()));

		SetMixFactor(0.0f);
	}

	void NextTransition(void)
	{
		std::size_t next = std::rand() % transitions.size();
		frag_subroutines.Assign(frag_transition, transitions[next]);
		direction.Set(std::rand() % 2);
	}

	void SetMixFactor(GLfloat factor)
	{
		const GLfloat margin = 0.2f;

		factor -= margin;
		factor /= (1.0f - 2.0f*margin);

		if(factor <= 0.0f)
		{
			if(prev_mix_factor > 0.0)
				NextTransition();
			factor = 0.0f;
		}
		else if(factor >= 1.0f)
		{
			if(prev_mix_factor < 1.0)
				NextTransition();
			factor = 1.0f;
		}
		mix_factor = factor;
		prev_mix_factor = factor;

		frag_subroutines.Apply();
	}
};

class Screen
{
private:
	Context gl;
	VertexArray vao;
	Buffer corners;
public:
	Screen(const Program& prog1, const Program& prog2)
	{
		// bind the VAO for the screen
		vao.Bind();

		// upload the position data
		corners.Bind(Buffer::Target::Array);
		{
			GLfloat screen_verts[8] = {
				-1.0f, -1.0f,
				-1.0f,  1.0f,
				 1.0f, -1.0f,
				 1.0f,  1.0f
			};
			Buffer::Data(Buffer::Target::Array, 8, screen_verts);

			VertexArrayAttrib attr(
				VertexArrayAttrib::GetCommonLocation(
					MakeGroup(prog1, prog2),
					"Position"
				)
			);
			attr.Setup<Vec2f>();
			attr.Enable();
		}
	}

	void Draw(void) const
	{
		vao.Bind();
		gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);
	}
};

class TransitionExample : public Example
{
private:
	// wrapper around the current OpenGL context
	Context gl;

	// the object drawing program
	DrawProgram draw_prog;

	// torus vertex attribute names and count and
	// a wraper for the torus builder, VAO and VBOs
	static GLuint torus_vert_attr_count(void) { return 4; }
	static const GLchar** torus_vert_attr_names(void)
	{
		static const GLchar* attrs[] = {"Position", "Normal", "Tangent", "TexCoord"};
		assert(sizeof(attrs)/sizeof(attrs[0]) == torus_vert_attr_count());
		return attrs;
	}
	shapes::ShapeWrapper torus;

	GLint next_free_tex_unit;

	// texture applied to the torus
	const GLint metal_tex_unit;
	MetalTexture metal_tex;

	// noise texture used in the transitions
	const GLint noise_tex_unit;
	NoiseTexture noise_tex;

	// the framebuffer for single-pass rendering of the two frames
	const GLint color_tex_unit, depth_tex_unit;
	DrawFramebuffer draw_fbo;

	DefaultFramebuffer def_fb;

	// the program that clears the background
	ClearProgram clear_prog;
	// program that renders the transition between the two frames
	TransitionProgram trans_prog;

	// rectangle covering the screen
	Screen screen;

	GLuint width, height;

	Mat4f projection_0, projection_1;
public:
	TransitionExample(void)
	 : draw_prog()
	 , torus(
		torus_vert_attr_names(),
		torus_vert_attr_count(),
		shapes::WickerTorus(1.5, 1.0, 0.03, 18, 36),
		draw_prog
	), next_free_tex_unit(0)
	 , metal_tex_unit(next_free_tex_unit++)
	 , metal_tex(metal_tex_unit)
	 , noise_tex_unit(next_free_tex_unit++)
	 , noise_tex(noise_tex_unit)
	 , color_tex_unit(next_free_tex_unit++)
	 , depth_tex_unit(next_free_tex_unit++)
	 , draw_fbo(color_tex_unit, depth_tex_unit)
	 , clear_prog()
	 , trans_prog()
	 , screen(clear_prog, trans_prog)
	{
		ProgramUniformSampler(draw_prog, "MetalTexture").Set(metal_tex_unit);
		ProgramUniform<Vec3f>(draw_prog, "LightPosition").Set(10.0f, 30.0f, 20.0f);
		ProgramUniform<Vec3f>(draw_prog, "Color[0]").Set(0.7f, 0.1f, 0.2f);
		ProgramUniform<Vec3f>(draw_prog, "Color[1]").Set(0.2f, 0.1f, 0.7f);

		ProgramUniform<Vec3f>(clear_prog, "Color1[0]").Set(0.9f, 0.4f, 0.5f);
		ProgramUniform<Vec3f>(clear_prog, "Color2[0]").Set(1.0f, 0.5f, 0.6f);
		ProgramUniform<Vec3f>(clear_prog, "Color1[1]").Set(0.5f, 0.4f, 0.9f);
		ProgramUniform<Vec3f>(clear_prog, "Color2[1]").Set(0.6f, 0.5f, 1.0f);

		ProgramUniformSampler(trans_prog, "FrameTex").Set(color_tex_unit);
		ProgramUniformSampler(trans_prog, "NoiseTex").Set(noise_tex_unit);

		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;

		draw_fbo.Resize(width, height);

		projection_0 = CamMatrixf::PerspectiveX(
			Degrees(70),
			width, height,
			1, 20
		);
		projection_1 = CamMatrixf::PerspectiveX(
			Degrees(24),
			width, height,
			1, 40
		);
	}

	void RenderFrames(double time)
	{
		Vec3f origin = Vec3f(0.0f, 0.0f, 0.0f);
		Vec3f target_0 = Vec3f(0.0f, 1.5f, 0.0f);
		auto camera_0 = CamMatrixf::Orbiting(
			target_0,
			GLfloat(4.0 + SineWave(time / 11.0)*2.0),
			FullCircles(time / 19.0),
			Degrees(SineWave(time / 20.0) * 30 + 35)
		);
		auto cm_0 = projection_0*camera_0;
		draw_prog.camera_matrix_0 = cm_0;
		draw_prog.camera_position_0 = camera_0.Position();
		clear_prog.origin_0 = (cm_0*Vec4f(origin, 1.0)).xy();

		Vec3f target_1 = Vec3f(0.0f,-0.1f, 0.1f);
		auto camera_1 = CamMatrixf::Orbiting(
			target_1,
			12.5f,
			FullCircles(time / 37.0),
			Degrees(SineWave(time / 11.0) * 85)
		);
		auto cm_1 = projection_1*camera_1;
		draw_prog.camera_matrix_1 = cm_1;
		draw_prog.camera_position_1 = camera_1.Position();
		clear_prog.origin_1 = (cm_1*Vec4f(origin, 1.0)).xy();

		gl.Viewport(width, height);
		gl.Disable(Capability::DepthTest);

		clear_prog.Use();
		screen.Draw();

		gl.Enable(Capability::DepthTest);
		gl.Clear().DepthBuffer();
		//
		draw_prog.model_matrix =
			ModelMatrixf::RotationY(FullCircles(time / 7.0))*
			ModelMatrixf::RotationX(Degrees(25))*
			ModelMatrixf::Translation(0.0, 0.0,-1.0);

		draw_prog.Use();
		torus.Use();
		torus.Draw();
	}

	void MergeFrames(double time)
	{
		gl.Viewport(width, height);
		gl.Disable(Capability::DepthTest);

		trans_prog.Use();
		trans_prog.SetMixFactor(GLfloat(-CosineWave(time / 6.0)*0.5+0.5));

		screen.Draw();
	}

	void Render(double time)
	{
		draw_fbo.Bind(Framebuffer::Target::Draw);
		RenderFrames(time);

		def_fb.Bind(Framebuffer::Target::Draw);
		MergeFrames(time);
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
	std::srand(unsigned(std::time(0)));
	return std::unique_ptr<Example>(new TransitionExample);
}

} // namespace oglplus
