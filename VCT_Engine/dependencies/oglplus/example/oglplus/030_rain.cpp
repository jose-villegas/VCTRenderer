/**
 *  @example oglplus/030_rain.cpp
 *  @brief Shows how to generate a texture simulating raindrops on water
 *
 *  @oglplus_screenshot{030_rain}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_texture{cloudy_day-cm_0}
 *  @oglplus_example_uses_texture{cloudy_day-cm_1}
 *  @oglplus_example_uses_texture{cloudy_day-cm_2}
 *  @oglplus_example_uses_texture{cloudy_day-cm_3}
 *  @oglplus_example_uses_texture{cloudy_day-cm_4}
 *  @oglplus_example_uses_texture{cloudy_day-cm_5}
 *
 *  @oglplus_example_uses_gl{GL_VERSION_3_3}
 *  @oglplus_example_uses_gl{GL_ARB_separate_shader_objects;GL_EXT_direct_state_access}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/shapes/plane.hpp>

#include <oglplus/images/image_spec.hpp>
#include <oglplus/images/load.hpp>

#include "example.hpp"

namespace oglplus {

class RippleTexProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;
		prog.AttachShader(VertexShader(
			ObjectDesc("Ripple texture vertex shader"),
			"#version 150\n"
			"void main(void)"
			"{"
			"	gl_Position = vec4(0.0, 0.0, 0.0, 1.0);"
			"}"
		));

		prog.AttachShader(GeometryShader(
			ObjectDesc("Ripple texture geometry shader"),
			"#version 150\n"
			"layout (points) in;"
			"layout (triangle_strip, max_vertices = 4) out;"

			"uniform int TexSize;"

			"out vec2 geomTexCoord;"

			"void make_vertex(vec2 pos, vec2 tc)"
			"{"
			"	gl_Position = vec4(pos, 0.0, 1.0);"
			"	geomTexCoord = tc;"
			"	EmitVertex();"
			"}"

			"void main(void)"
			"{"
			"	float rts = TexSize;"
			"	make_vertex(vec2(-1.0,-1.0), vec2(  0,   0));"
			"	make_vertex(vec2(-1.0, 1.0), vec2(  0, rts));"
			"	make_vertex(vec2( 1.0,-1.0), vec2(rts,   0));"
			"	make_vertex(vec2( 1.0, 1.0), vec2(rts, rts));"
			"	EndPrimitive();"
			"}"
		));

		prog.AttachShader(FragmentShader(
			ObjectDesc("Ripple texture fragment shader"),
			"#if GL_ARB_explicit_attrib_location\n"
			"#version 150\n"
			"#extension GL_ARB_explicit_attrib_location : enable\n"
			"#else\n"
			"#version 330\n"
			"#endif\n"

			"uniform ivec2 NewDrop;"
			"uniform sampler2D Tex1, Tex2;"
			"uniform int TexSize;"

			"in vec2 geomTexCoord;"

			"layout (location = 0) out vec4 fragBump;"
			"layout (location = 1) out float fragHeight;"

			"ivec2 wrap_tc(ivec2 tc)"
			"{"
			"	if(tc.x < 0) tc.x = TexSize-1;"
			"	if(tc.x >= TexSize) tc.x = 0;"
			"	if(tc.y < 0) tc.y = TexSize-1;"
			"	if(tc.y >= TexSize) tc.y = 0;"
			"	return tc;"
			"}"

			"float height_at(sampler2D tex, ivec2 tc, float factor)"
			"{"
			"	return texelFetch(tex, wrap_tc(tc), 0).r * factor;"
			"}"

			"void main(void)"
			"{"
			"	ivec2 TC = ivec2(geomTexCoord);"

			"	float  ch = height_at(Tex2, TC, 1.0);"
			"	float xp1 = height_at(Tex2, TC+ivec2( 1, 0), 0.25);"
			"	float xm1 = height_at(Tex2, TC+ivec2(-1, 0), 0.25);"
			"	float yp1 = height_at(Tex2, TC+ivec2( 0, 1), 0.25);"
			"	float ym1 = height_at(Tex2, TC+ivec2( 0,-1), 0.25);"

			"	fragHeight = xp1 + xm1 + yp1 + ym1;"
			"	fragHeight += height_at(Tex2, TC+ivec2( 1,-1), 0.25);"
			"	fragHeight += height_at(Tex2, TC+ivec2( 1, 1), 0.25);"
			"	fragHeight += height_at(Tex2, TC+ivec2(-1,-1), 0.25);"
			"	fragHeight += height_at(Tex2, TC+ivec2(-1, 1), 0.25);"
			"	fragHeight -= height_at(Tex1, TC, 1.0);"
			"	vec2 d = NewDrop - TC;"
			"	fragHeight += length(d)<8?1.0:0.0;"
			"	vec3 fragNormal = vec3("
			"		(xm1 - ch) + (ch - xp1),"
			"		(ym1 - ch) + (ch - yp1),"
			"		0.1"
			"	);"
			"	fragBump = vec4("
			"		normalize(fragNormal),"
			"		fragHeight"
			"	);"
			"}"
		));

		prog.Link();
		prog.Use();

		return prog;
	}

	Program& self(void) { return *this; }
public:
	ProgramUniformSampler tex_1, tex_2;
	ProgramUniform<Vector<GLint,2>> new_drop;

	RippleTexProg(GLuint ripple_tex_size)
	 : Program(make())
	 , tex_1(self(), "Tex1")
	 , tex_2(self(), "Tex2")
	 , new_drop(self(), "NewDrop")
	{
		ProgramUniform<GLint>(self(), "TexSize").Set(ripple_tex_size);
	}
};

class RippleTexHolder
{
private:
	const GLuint first_tex_unit;
	const GLuint nhm;
	Array<Texture> height_maps;
	Texture bump_map;
	GLuint curr;
public:
	RippleTexHolder(GLuint ripple_tex_size, GLuint tex_unit)
	 : first_tex_unit(tex_unit)
	 , nhm(3)
	 , height_maps(nhm)
	 , curr(0)
	{
		std::vector<GLfloat> v(ripple_tex_size*ripple_tex_size, 0.0f);
		for(GLuint i=0; i!=nhm; ++i)
		{
			Texture::Active(first_tex_unit+i);
			height_maps[i]
				<< Texture::Target::_2D
				<< TextureFilter::Nearest
				<< TextureWrap::Repeat
				<< images::ImageSpec(
					ripple_tex_size,
					ripple_tex_size,
					Format::Red,
					v.data()
				);
		}

		Texture::Active(first_tex_unit+nhm);
		bump_map
			<< Texture::Target::_2D
			<< TextureFilter::Linear
			<< TextureWrap::Repeat
			<< images::ImageSpec(
				ripple_tex_size,
				ripple_tex_size,
				Format::RGBA,
				DataType::UnsignedByte
			);
	}

	void Swap(void)
	{
		++curr;
	}

	GLuint TexUnit1(void) const
	{
		return first_tex_unit + (curr + 0) % nhm;
	}

	GLuint TexUnit2(void) const
	{
		return first_tex_unit + (curr + 1) % nhm;
	}

	Reference<Texture> CurrentHeightMap(void) const
	{
		return height_maps[(curr + 2) % nhm];
	}

	GLuint BumpMapUnit(void) const
	{
		return first_tex_unit + nhm;
	}

	Reference<Texture> BumpMap(void) const
	{
		return bump_map;
	}
};

class RippleGenerator
{
private:
	Context gl;
	GLuint size;
	VertexArray vao;
	Framebuffer fbo;
	RippleTexProg prog;
	RippleTexHolder holder;
public:
	RippleGenerator(GLuint first_tex_unit)
	 : size(1024)
	 , prog(size)
	 , holder(size, first_tex_unit)
	{
		fbo.Bind(Framebuffer::Target::Draw);
		Framebuffer::AttachColorTexture(
			Framebuffer::Target::Draw,
			0,
			holder.BumpMap(),
			0
		);

		vao.Bind();
	}

	void Update(void)
	{
		gl.Viewport(size, size);

		fbo.Bind(Framebuffer::Target::Draw);
		Framebuffer::AttachColorTexture(
			Framebuffer::Target::Draw,
			1,
			holder.CurrentHeightMap(),
			0
		);
		Context::ColorBuffer draw_buffs[2] = {
			FramebufferColorAttachment::_0,
			FramebufferColorAttachment::_1
		};
		gl.DrawBuffers(draw_buffs);

		prog.Use();
		prog.tex_1.Set(holder.TexUnit1());
		prog.tex_2.Set(holder.TexUnit2());
		prog.new_drop.Set(std::rand()%size, std::rand()%size);

		vao.Bind();
		gl.DrawArrays(PrimitiveType::Points, 0, 1);

		holder.Swap();
	}

	GLuint TexUnit(void) const
	{
		return holder.BumpMapUnit();
	}
};

template <typename ShapeBuilder>
class Shape
{
protected:
	ShapeBuilder make_shape;
	shapes::DrawingInstructions shape_instr;
	typename ShapeBuilder::IndexArray shape_indices;

	Context gl;

	VertexArray vao;

	const GLuint nva;

	Array<Buffer> vbos;

public:
	Shape(const Program& prog, const ShapeBuilder& builder)
	 : make_shape(builder)
	 , shape_instr(make_shape.Instructions())
	 , shape_indices(make_shape.Indices())
	 , nva(4)
	 , vbos(nva+1)
	{
		vao.Bind();

		typename ShapeBuilder::VertexAttribs vert_attr_info;
		const GLchar* vert_attr_name[] = {
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
			try
			{
				vbos[va].Bind(Buffer::Target::Array);
				GLuint npv = getter(make_shape, data);
				Buffer::Data(Buffer::Target::Array, data);
				VertexArrayAttrib attr(prog, name);
				attr.Setup<GLfloat>(npv);
				attr.Enable();
			}
			catch(Error&)
			{ }
		}
		vbos[nva].Bind(Buffer::Target::ElementArray);
		Buffer::Data(Buffer::Target::ElementArray, shape_indices);
		shape_indices.clear();
	}

	void Draw(void)
	{
		vao.Bind();
		gl.FrontFace(make_shape.FaceWinding());
		shape_instr.Draw(shape_indices);
	}
};

class WaterProg
 : public Program
{
private:
	static Program make(void)
	{
		Program prog;
		prog.AttachShader(VertexShader(
			ObjectDesc("Water vertex shader"),
			"#version 150\n"

			"uniform mat4 ProjectionMatrix, CameraMatrix;"
			"uniform vec3 LightPosition, CameraPosition;"
			"mat4 Matrix = ProjectionMatrix*CameraMatrix;"

			"in vec3 Position;"
			"in vec3 Normal;"
			"in vec3 Tangent;"
			"in vec2 TexCoord;"

			"out vec3 vertNormal, vertTangent, vertBitangent;"
			"out vec3 vertLightDir, vertViewDir;"
			"out vec2 vertTexCoord;"

			"void main(void)"
			"{"
			"	gl_Position = Matrix * vec4(Position, 1.0);"
			"	vertNormal = Normal;"
			"	vertTangent = Tangent;"
			"	vertBitangent = cross(Normal, Tangent);"
			"	vertLightDir = LightPosition - Position;"
			"	vertViewDir = CameraPosition - Position;"
			"	vertTexCoord = TexCoord * 16.0;"
			"}"
		));

		prog.AttachShader(FragmentShader(
			ObjectDesc("Water fragment shader"),
			"#if GL_ARB_explicit_attrib_location\n"
			"#version 150\n"
			"#extension GL_ARB_explicit_attrib_location : enable\n"
			"#else\n"
			"#version 330\n"
			"#endif\n"

			"uniform sampler2D RippleTex;"
			"uniform samplerCube EnvTex;"

			"in vec3 vertNormal, vertTangent, vertBitangent;"
			"in vec3 vertLightDir, vertViewDir;"
			"in vec2 vertTexCoord;"

			"layout (location = 0) out vec4 fragColor;"

			"const vec3 LightColor = vec3(1.0, 1.0, 0.95);"
			"const vec3 BgColor = vec3(0.4, 0.4, 0.4);"

			"void main(void)"
			"{"
			"	vec4 texBump = texture(RippleTex, vertTexCoord);"
			"	vec3 fragNormal = normalize("
			"		texBump.x * vertTangent+"
			"		texBump.y * vertBitangent+"
			"		texBump.z * vertNormal"
			"	);"

			"	vec3 fragLightDir = normalize(vertLightDir);"
			"	vec3 fragLightRefl = reflect(-fragLightDir, fragNormal);"

			"	vec3 fragViewDir = normalize(vertViewDir);"
			"	vec3 fragViewRefl = reflect(-fragViewDir, fragNormal);"
			"	vec3 fragViewRefr = refract("
			"		-normalize(fragViewDir+vec3(0.0, 1.0, 0.0)),"
			"		fragNormal,"
			"		1.2"
			"	);"

			"	float Diffuse = max(dot(fragNormal, fragLightDir), 0.0);"
			"	float Specular = clamp(pow(dot(fragViewDir, fragLightRefl)+0.1, 64.0), 0.0, 2.0);"
			"	float Transparency = max(dot(fragNormal, fragViewDir)+0.3, 0.0);"

			"	float Visibility = min(16.0 / dot(vertViewDir, vertViewDir), 1.0);"

			"	vec3 SkyColor = texture(EnvTex, fragViewRefl).rgb;"
			"	vec3 GroundColor = texture(EnvTex, fragViewRefr).rgb;"

			"	vec3 WaterColor = "
			"		mix("
			"			0.7*SkyColor,"
			"			0.8*GroundColor*LightColor*Diffuse,"
			"			Transparency"
			"		)+"
			"		0.2*LightColor*Diffuse+"
			"		0.05*LightColor*Specular;"

			"	fragColor = vec4("
			"		mix(BgColor, WaterColor, Visibility),"
			"		1.0"
			"	);"
			"}"
		));
		prog.Link();
		prog.Use();
		return prog;
	}

	Program& self(void){return *this;}
public:
	ProgramUniform<Mat4f> projection_matrix, camera_matrix;
	ProgramUniform<Vec3f> camera_position, light_position;
	ProgramUniformSampler ripple_tex, env_tex;

	WaterProg(void)
	 : Program(make())
	 , projection_matrix(self(), "ProjectionMatrix")
	 , camera_matrix(self(), "CameraMatrix")
	 , camera_position(self(), "CameraPosition")
	 , light_position(self(), "LightPosition")
	 , ripple_tex(self(), "RippleTex")
	 , env_tex(self(), "EnvTex")
	{ }
};

class EnvMap
 : public Texture
{
private:
	GLuint _tex_unit;
public:
	EnvMap(GLuint tex_unit)
	 : _tex_unit(tex_unit)
	{
		Texture::Active(_tex_unit);

		*this	<< Texture::Target::CubeMap
			<< TextureFilter::Linear
			<< TextureWrap::ClampToEdge;

		Texture::CubeMapFace(0) <<
			images::LoadTexture("cloudy_day-cm_0", false, false);
		Texture::CubeMapFace(1) <<
			images::LoadTexture("cloudy_day-cm_1", false, false);
		Texture::CubeMapFace(2) <<
			images::LoadTexture("cloudy_day-cm_2", false, false);
		Texture::CubeMapFace(3) <<
			images::LoadTexture("cloudy_day-cm_3", false, false);
		Texture::CubeMapFace(4) <<
			images::LoadTexture("cloudy_day-cm_4", false, false);
		Texture::CubeMapFace(5) <<
			images::LoadTexture("cloudy_day-cm_5", false, false);
	}

	GLuint TexUnit(void) const
	{
		return _tex_unit;
	}
};

class RainExample : public Example
{
private:
	Context gl;

	GLuint width, height;

	EnvMap env_map;

	RippleGenerator ripples;

	WaterProg water_prog;

	Shape<shapes::Plane> water;
public:
	RainExample(void)
	 : env_map(0)
	 , ripples(1)
	 , water_prog()
	 , water(water_prog, shapes::Plane(Vec3f(40,0,0), Vec3f(0,0,-40)))
	{
		water_prog.light_position.Set(2.0f, 10.0f, -4.0f);

		water_prog.ripple_tex.Set(ripples.TexUnit());
		water_prog.env_tex.Set(env_map.TexUnit());


		gl.ClearColor(0.4f, 0.4f, 0.4f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;

		water_prog.projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				float(width)/float(height),
				1.0, 100.0
			)
		);
	}

	void Render(double time)
	{
		ripples.Update();

		DefaultFramebuffer().Bind(Framebuffer::Target::Draw);
		gl.DrawBuffer(ColorBuffer::BackLeft);
		gl.Viewport(width, height);
		gl.Clear().ColorBuffer().DepthBuffer();

		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			GLfloat(6.0 + SineWave(time / 31.0)),
			FullCircles(time / 17.0),
			Degrees(50 + SineWave(time / 21.0) * 35)
		);

		water_prog.Use();
		water_prog.camera_matrix.Set(camera);
		water_prog.camera_position.Set(camera.Position());

		water.Draw();
	}

	bool Continue(const ExampleClock& clock)
	{
		return clock.RealTime().Minutes() < 2.0;
	}

	double HeatUpTime(void) const
	{
		return 0.0;
	}

	double ScreenshotTime(void) const
	{
		return 3.0;
	}

	double FrameTime(void) const
	{
		return 1.0/60.0;
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
	return std::unique_ptr<Example>(new RainExample);
}

} // namespace oglplus
