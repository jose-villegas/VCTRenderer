/**
 *  @example oglplus/027_flow.cpp
 *  @brief Shows how to generate a normal map simulating flowing liquid
 *
 *  @oglplus_screenshot{027_flow}
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  @oglplus_example_uses_texture{flow_map}
 *  @oglplus_example_uses_texture{flower_glass}
 *
 *  @oglplus_example_uses_cpp_feat{SCOPED_ENUMS}
 *  @oglplus_example_uses_gl{GL_VERSION_3_2}
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>

#include <oglplus/bound/texture.hpp>

#include <oglplus/shapes/screen.hpp>
#include <oglplus/shapes/plane.hpp>
#include <oglplus/shapes/wrapper.hpp>

#include <oglplus/images/load.hpp>

#include <oglplus/opt/list_init.hpp>

#include "example.hpp"

namespace oglplus {

class FlowMapProg
 : public Program
{
private:
	static Program make_prog(void)
	{
		Program prog(ObjectDesc("Flow map"));
		prog.AttachShader(VertexShader(
			ObjectDesc("Flow map vertex"),
			StrCRef(
			"#version 140\n"

			"in vec4 Position;"
			"in vec2 TexCoord;"

			"out vec2 vertTexCoord;"

			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"	vertTexCoord = TexCoord;"
			"}"
			)
		)).AttachShader(FragmentShader(
			ObjectDesc("Flow map fragment"),
			StrCRef(
			"#if GL_ARB_explicit_attrib_location\n"
			"#version 140\n"
			"#extension GL_ARB_explicit_attrib_location : enable\n"
			"#else\n"
			"#version 330\n"
			"#endif\n"

			"uniform sampler2D HMap1, HMap2;"
			"uniform sampler2D FlowMap;"
			"uniform int HMapSize;"
			"uniform float Time;"

			"float TS = 1.0/HMapSize;"

			"in vec2 vertTexCoord;"

			"layout (location = 0) out vec4 fragBump;"
			"layout (location = 1) out float fragHeight;"

			"ivec2 clamp_tc(ivec2 tc)"
			"{"
			"	return clamp(tc, ivec2(0, 0), ivec2(HMapSize, HMapSize));"
			"}"

			"float height_at("
			"	sampler2D tex, "
			"	float xoffs,"
			"	float yoffs,"
			"	float steer,"
			"	float flow, "
			"	float factor"
			")"
			"{"
			"	vec2 TC = vertTexCoord + vec2(xoffs, yoffs)*TS;"
			"	return texture(tex, TC+vec2(steer, flow)*TS).r*factor;"
			"}"

			"void main(void)"
			"{"
			"	vec2 FC = gl_FragCoord.xy+vec2(-HMapSize/2, -HMapSize+4);"
			"	float s = 2.71*sin(1.1*Time+0.03*length(FC));"
			"	float f = 1.62*sin(2.0*Time-0.05*length(FC));"
			"	vec2 TC = vertTexCoord+TS*vec2(s, f)+vec2(0.0, Time*0.1);"
			"	vec4 fm = texture(FlowMap, TC);"

			"	s = 0.1*s+(0.5-fm.r)*3.0;"
			"	f = 0.3*f+(fm.g-0.5)*4.0;"

			"	float  ch = height_at(HMap2, 0, 0, s, f, 1.00);"
			"	s *= 1.5;"
			"	f *= 1.618;"
			"	float xp1 = height_at(HMap2, 1, 0, s, f, 0.25);"
			"	float xm1 = height_at(HMap2,-1, 0, s, f, 0.25);"
			"	float yp1 = height_at(HMap2, 0, 1, s, f, 0.25);"
			"	float ym1 = height_at(HMap2, 0,-1, s, f, 0.25);"

			"	vec3 fragNormal = vec3("
			"		(xm1 - ch) + (ch - xp1),"
			"		(ym1 - ch) + (ch - yp1),"
			"		0.2"
			"	);"

			"	fragHeight  = xp1 + xm1 + yp1 + ym1;"
			"	fragHeight += height_at(HMap2, 1,-1, s, f, 0.25);"
			"	fragHeight += height_at(HMap2, 1, 1, s, f, 0.25);"
			"	fragHeight += height_at(HMap2,-1,-1, s, f, 0.25);"
			"	fragHeight += height_at(HMap2,-1, 1, s, f, 0.25);"

			"	f *= 1.618;"
			"	s -= fragNormal.x*3.0;"
			"	fragHeight -= height_at(HMap1, 0, 0, s, f, 1.00);"

			"	fragHeight += (fm.b-0.5)*0.09;"
			"	if(FC.y > 0.0)"
			"		fragHeight += min(pow(sin(Time+length(10*FC)), 16.0), 0.3);"

			"	fragBump = vec4(normalize(fragNormal),clamp(fragHeight, 0.0, 1.0));"
			"}"
			)
		)).Link().Use();

		return std::move(prog);
	}

	Program& prog(void) { return *this; }
public:
	Uniform<SLtoCpp<SLDataType::Sampler2D>> hmap_1, hmap_2;
	Uniform<GLfloat> time;

	FlowMapProg(GLint tex_size)
	 : Program(make_prog())
	 , hmap_1(prog(), "HMap1")
	 , hmap_2(prog(), "HMap2")
	 , time(prog(), "Time")
	{
		Uniform<GLint>(prog(), "HMapSize").Set(tex_size);
	}
};

class FlowMapHolder
{
private:
	const GLuint first_tex_unit;
	const GLuint nhm;
	Texture flow_map;
	Array<Texture> height_maps;
	Texture bump_map;
	GLuint curr;
public:
	FlowMapHolder(
		GLuint flow_tex_size,
		GLuint tex_unit,
		const images::Image& flow_map_image
	): first_tex_unit(tex_unit)
	 , nhm(3)
	 , height_maps(nhm)
	 , curr(0)
	{
		std::vector<GLfloat> v(flow_tex_size*flow_tex_size, 0.0f);
		for(GLuint i=0; i!=nhm; ++i)
		{
			Texture::Active(first_tex_unit+i);
			Context::Bound(Texture::Target::_2D, height_maps[i])
				.MinFilter(TextureMinFilter::Linear)
				.MagFilter(TextureMagFilter::Linear)
				.WrapS(TextureWrap::ClampToBorder)
				.WrapT(TextureWrap::ClampToBorder)
				.Image2D(
					0,
					PixelDataInternalFormat::Red,
					flow_tex_size, flow_tex_size,
					0,
					PixelDataFormat::Red,
					PixelDataType::Float,
					v.data()
				);
		}

		Texture::Active(BumpMapUnit());
		Context::Bound(Texture::Target::_2D, bump_map)
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::ClampToEdge)
			.WrapT(TextureWrap::ClampToEdge)
			.Image2D(
				0,
				PixelDataInternalFormat::RGBA,
				flow_tex_size, flow_tex_size,
				0,
				PixelDataFormat::RGBA,
				PixelDataType::UnsignedByte,
				nullptr
			);

		Texture::Active(FlowMapUnit());
		Context::Bound(Texture::Target::_2D, flow_map)
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::Repeat)
			.WrapT(TextureWrap::Repeat)
			.Image2D(flow_map_image);
	}

	void Swap(void)
	{
		++curr;
	}

	GLuint FlowMapUnit(void) const
	{
		return first_tex_unit + nhm + 1;
	}

	GLuint HMapUnit1(void) const
	{
		return first_tex_unit + (curr + 0) % nhm;
	}

	GLuint HMapUnit2(void) const
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

class FlowSimulator
{
private:
	Context gl;
	GLuint size;
	Framebuffer fbo;
	FlowMapProg prog;
	FlowMapHolder holder;
	shapes::ShapeWrapper screen;
public:
	FlowSimulator(
		const images::Image& flow_map_image,
		GLuint first_tex_unit
	): size(1024)
	 , prog(size)
	 , holder(size, first_tex_unit, flow_map_image)
	 , screen(List("Position")("TexCoord").Get(), shapes::Screen(), prog)
	{
	 	Uniform<SLtoCpp<SLDataType::Sampler2D>> flow_map(prog, "FlowMap");
		flow_map.Set(holder.FlowMapUnit());

		fbo.Bind(Framebuffer::Target::Draw);
		Framebuffer::AttachColorTexture(
			Framebuffer::Target::Draw,
			0,
			holder.BumpMap(),
			0
		);
	}

	void Update(double time)
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
		prog.hmap_1.Set(holder.HMapUnit1());
		prog.hmap_2.Set(holder.HMapUnit2());
		prog.time.Set(GLfloat(time));

		screen.Use();

		gl.Disable(Capability::DepthTest);
		screen.Draw();
		gl.Enable(Capability::DepthTest);

		holder.Swap();
	}

	GLuint TexUnit(void) const
	{
		return holder.BumpMapUnit();
	}
};


class ScreenProg
 : public Program
{
private:
	static Program make_prog(void)
	{
		Program prog(ObjectDesc("Screen"));

		prog.AttachShader(VertexShader(
			ObjectDesc("Screen vertex"),
			StrCRef(
			"#version 140\n"

			"in vec4 Position;"
			"in vec2 TexCoord;"

			"out vec2 vertTexCoord;"

			"void main(void)"
			"{"
			"	gl_Position = Position;"
			"	vertTexCoord = TexCoord;"
			"}"
			)
		)).AttachShader(FragmentShader(
			ObjectDesc("Screen fragment"),
			StrCRef(
			"#version 140\n"

			"uniform sampler2D Background, NormalMap;"

			"in vec2 vertTexCoord;"

			"out vec3 fragColor;"

			"void main(void)"
			"{"
			"	vec4 nm = texture(NormalMap, vertTexCoord);"
			"	vec2 offs = nm.xy*0.05+vec2(0.004, 0.004)*nm.w;"
			"	vec4 c = texture(Background, vertTexCoord+offs);"
			"	float l = clamp(nm.w+pow(dot(nm.xy,vec2(0.2,0.2)), 9.0), 0.3, 1.1);"
			"	fragColor = c.rgb*mix(1.2, 0.8, l);"
			"}"
			)
		)).Link().Use();

		return std::move(prog);
	}

	Program& prog(void) { return *this; }
public:
	Uniform<SLtoCpp<SLDataType::Sampler2D>> background, normal_map;

	ScreenProg(void)
	 : Program(make_prog())
	 , background(prog(), "Background")
	 , normal_map(prog(), "NormalMap")
	{ }
};

class FlowExample : public Example
{
private:
	Context gl;

	GLuint width, height;

	FlowSimulator flow;

	Texture background;

	DefaultFramebuffer dfb;

	ScreenProg screen_prog;

	shapes::ShapeWrapper screen;
public:
	FlowExample(void)
	 : gl()
	 , flow(images::LoadTexture("flow_map"), 1)
	 , screen(
		List("Position")("TexCoord").Get(),
		shapes::Screen(),
		screen_prog
	)
	{
		Texture::Active(0);
		gl.Bound(Texture::Target::_2D, background)
			.Image2D(images::LoadTexture("flower_glass"))
			.MinFilter(TextureMinFilter::Linear)
			.MagFilter(TextureMagFilter::Linear)
			.WrapS(TextureWrap::MirroredRepeat)
			.WrapT(TextureWrap::MirroredRepeat);

		screen_prog.background.Set(0);
		screen_prog.normal_map.Set(flow.TexUnit());

		gl.ClearColor(0.4f, 0.4f, 0.4f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
	}

	void Reshape(GLuint vp_width, GLuint vp_height)
	{
		width = vp_width;
		height = vp_height;
	}

	void Render(double time)
	{
		flow.Update(time);

		dfb.Bind(Framebuffer::Target::Draw);
		gl.DrawBuffer(ColorBuffer::BackLeft);
		gl.Viewport(width, height);
		gl.Clear().ColorBuffer().DepthBuffer();

		screen_prog.Use();
		screen.Use();
		screen.Draw();
	}

	bool Continue(const ExampleClock& clock)
	{
		return clock.RealTime().Minutes() < 1.0;
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
		return 1.0/30.0;
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
	return std::unique_ptr<Example>(new FlowExample);
}

} // namespace oglplus
