/**
 *  @example standalone/004_any_text_rendering.cpp
 *  @brief Shows the usage of OGLplus' text rendering utilities
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include "glut_glew_example.hpp"

#include <oglplus/all.hpp>
#include <oglplus/images/png.hpp>

#include <oglplus/text/stb_truetype.hpp>
#include <oglplus/text/any.hpp>

#include <sstream>
#include <iomanip>


class STBTruetypeExample
 : public oglplus::StandaloneExample
{
private:
	oglplus::Context gl;

	typedef oglplus::text::AnyTextRendering TextRendering;

	TextRendering tr;

	TextRendering::Font font;

	TextRendering::Layout oglp_layout;
	TextRendering::Layout desc_layout;

	TextRendering::Layout time_layout;

	std::string time_str;
	std::stringstream text_stream;

	TextRendering::Renderer rndr;

public:
	STBTruetypeExample(int argc, const char** argv)
	 : gl()
	 , tr(oglplus::text::STBTrueTypeRendering(0, 1, 2))
	 , font(tr.LoadFont((argc>1)?argv[1]:"FreeSans"))
	 , oglp_layout(tr.MakeLayout(font, "OGLplus"))
#if !OGLPLUS_NO_UNICODE_LITERALS
	 , desc_layout(tr.MakeLayout(font, u8"a C++ wrapper for OpenGL©"))
#else
	 , desc_layout(tr.MakeLayout(font, "a C++ wrapper for OpenGL(c)"))
#endif
	 , time_layout(tr.MakeLayout(font, 25))
	 , rndr(tr.GetRenderer(
			oglplus::FragmentShader(
				oglplus::ObjectDesc("Pixel color"),
				oglplus::StrCRef(
				"#version 140\n"
				"vec4 PixelColor("
				"	vec4 TexelColor,"
				"	vec3 GlyphPosition,"
				"	float GlyphXOffset,"
				"	vec2 GlyphExtent,"
				"	vec2 GlyphCoord,"
				"	float LayoutWidth"
				")"
				"{"
				"	float g = GlyphXOffset / LayoutWidth;"
				"	float b = GlyphCoord.y;"
				"	vec3 Color = mix("
				"		vec3(1.0, 0.2, 0.2+0.8*b), "
				"		vec3(0.2, 1.0, 0.2+0.8*b), "
				"		g"
				"	);"
				"	return vec4(Color, TexelColor.r);"
				"}")
			)
		)
	)
	{
		using namespace oglplus;

		rndr.Use();

		gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		gl.Enable(Capability::Blend);
		gl.BlendFunc(
			BlendFunction::SrcAlpha,
			BlendFunction::DstAlpha
		);
	}

	void Reshape(void)
	{
		using namespace oglplus;

		gl.Viewport(Width(), Height());

		rndr.SetProjection(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				Width()/Height(),
				1, 60
			)
		);
	}

	void Render(void)
	{
		using namespace oglplus;

		gl.Clear().ColorBuffer().DoIt();

		GLfloat t = GLfloat(FrameTime());

		rndr.Use();

		text_stream.str(std::string());
		text_stream
			<< "Time: "
			<< std::fixed
			<< std::setw(5)
			<< std::setprecision(3)
			<< t << " [s]";
		if(time_str != text_stream.str())
		{
			time_str = text_stream.str();
			time_layout.Set(time_str);
		}

		rndr.SetCamera(
			CamMatrixf::Orbiting(
				Vec3f(),
				GLfloat(9.0 + SineWave(t / 7.0)*3.0),
				-FullCircles(t / 17.0),
				Degrees(SineWave(t / 21.0) * 35)
			)
		);

		rndr.SetLayoutTransform(ModelMatrixf::Translation(-3.0f, 0.7f, 1.1f));
		rndr.Render(oglp_layout);

		rndr.SetLayoutTransform(ModelMatrixf::Translation(-6.0f,-0.7f, 0.0f));
		rndr.Render(desc_layout);

		rndr.SetLayoutTransform(
			ModelMatrixf::Translation(-4.0f,-2.0f,-0.5f)*
			ModelMatrixf::Scale(0.7f, 0.7f, 0.5f)
		);
		rndr.Render(time_layout);
	}
};

int main(int argc, char* argv[])
{
	return oglplus::GlutGlewMain<STBTruetypeExample>(
		"Example of usage of OGLplus' truetype text rendering",
		argc, argv
	);
}

