/**
 *  @example standalone/004_stbttf_text_rendering.cpp
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

#include <sstream>
#include <iomanip>


class STBTruetypeExample
 : public oglplus::StandaloneExample
{
private:
	oglplus::Context gl;

	typedef oglplus::text::STBTrueTypeRendering TextRendering;

	TextRendering tr;

	TextRendering::Font font;

	TextRendering::Layout oglp_layout;
	TextRendering::Layout desc_layout;

	TextRendering::Layout time_layout;
	std::string time_str;

	std::stringstream text_stream;

	TextRendering::CustomRenderer rndr;

	oglplus::ProgramUniform<oglplus::Mat4f>
		rndr_projection_matrix,
		rndr_camera_matrix,
		rndr_layout_matrix;

	oglplus::ProgramUniform<GLfloat>
		rndr_time;

	oglplus::ProgramUniform<GLint>
		rndr_axis;
public:
	STBTruetypeExample(int argc, const char** argv)
	 : gl()
	 , tr(0, 1, 2)
	 , font(tr.LoadFont((argc>1)?argv[1]:"FreeSans"))
	 , oglp_layout(tr.MakeLayout(font, "OGLplus"))
#if !OGLPLUS_NO_UNICODE_LITERALS
	 , desc_layout(tr.MakeLayout(font, u8"a C++ wrapper for OpenGL©"))
#else
	 , desc_layout(tr.MakeLayout(font, "a C++ wrapper for OpenGL(c)"))
#endif
	 , time_layout(tr.MakeLayout(font, 25))
	 , rndr(tr.GetRenderer(
			oglplus::GeometryShader(
				oglplus::ObjectDesc("Layout transform"),
				oglplus::StrCRef(
				"#version 150\n"
				"uniform mat4  ProjectionMatrix,CameraMatrix,LayoutMatrix;"
				"mat4 Matrix = ProjectionMatrix*CameraMatrix*LayoutMatrix;"

				"vec4 TransformLayout(vec3 GlyphPosition)"
				"{"
				"	return Matrix * vec4(GlyphPosition, 1.0);"
				"}")
			),
			oglplus::GeometryShader(
				oglplus::ObjectDesc("Glyph transform"),
				oglplus::StrCRef("#version 150\n"
				"uniform float Time;"
				"uniform int Axis;"

				"vec3 TransformGlyph("
				"	vec4 LogicalMetrics,"
				"	vec4 InkMetrics,"
				"	vec2 Pos,"
				"	float XOffs,"
				"	float LayoutWidth,"
				"	int Idx"
				")"
				"{"
				"	float a = Idx*0.3+Time*2.4;"
				"	float cx = cos(a);"
				"	float sx = sin(a);"
				"	mat3 m;"
				"	vec3 v;"
				"	vec3 o = vec3(XOffs, 0, 0);"
				"	vec3 p = vec3(Pos, 0);"
				"	if(Axis == 0)"
				"	{"
				"		m = mat3("
				"			  1,  0,  0,"
				"			  0, cx, sx,"
				"			  0,-sx, cx "
				"		);"
				"		v = vec3(0, (InkMetrics.z-InkMetrics.w)*0.5, 0);"
				"	}"
				"	else if(Axis == 1)"
				"	{"
				"		m = mat3("
				"			 cx,  0,-sx,"
				"			  0,  1,  0,"
				"			 sx,  0, cx "
				"		);"
				"		v = vec3((InkMetrics.y-InkMetrics.x)*0.5, 0, 0);"
				"	}"
				"	else if(Axis == 2)"
				"	{"
				"		m = mat3("
				"			  1,  0,  0,"
				"			  0,-cx,-sx,"
				"			  0, sx,-cx "
				"		);"
				"		v = vec3(0, 0, 0);"
				"	}"
				"	return m*(p-v)+(o+v);"
				"}")
			),
			oglplus::FragmentShader(
				oglplus::ObjectDesc("Pixel color"),
				oglplus::StrCRef("#version 150\n"
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
				"	vec3 Color = mix("
				"		vec3(1.0, 0.2, 0.2), "
				"		vec3(0.2, 0.4, 1.0), "
				"		g"
				"	);"
				"	return vec4(Color, TexelColor.r);"
				"}")
			)
		)
	), rndr_projection_matrix(rndr.GetUniform<oglplus::Mat4f>("ProjectionMatrix"))
	 , rndr_camera_matrix(rndr.GetUniform<oglplus::Mat4f>("CameraMatrix"))
	 , rndr_layout_matrix(rndr.GetUniform<oglplus::Mat4f>("LayoutMatrix"))
	 , rndr_time(rndr.GetUniform<GLfloat>("Time"))
	 , rndr_axis(rndr.GetUniform<GLint>("Axis"))
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

		rndr_projection_matrix.Set(
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

		rndr_time.Set(t);

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

		rndr_camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				GLfloat(9.0 + SineWave(t / 7.0)*3.0),
				-FullCircles(t / 17.0),
				Degrees(SineWave(t / 21.0) * 35)
			)
		);

		rndr_axis.Set(0);
		rndr_layout_matrix.Set(ModelMatrixf::Translation(-3.0f, 0.7f, 0.1f));
		rndr.Render(oglp_layout);

		rndr_axis.Set(1);
		rndr_layout_matrix.Set(ModelMatrixf::Translation(-8.0f,-0.7f, 0.0f));
		rndr.Render(desc_layout);

		rndr_axis.Set(2);
		rndr_layout_matrix.Set(
			ModelMatrixf::Translation(-4.0f,-2.0f, 0.0f)*
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

