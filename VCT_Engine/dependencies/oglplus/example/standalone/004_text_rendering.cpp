/**
 *  @example standalone/004_text_rendering.cpp
 *  @brief Shows usage of the NV_path_rendering extension for rendering text
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "glut_glew_example.hpp"

#include <oglplus/all.hpp>
#include <oglplus/ext/ARB_compatibility.hpp>
#include <oglplus/ext/NV_path_rendering.hpp>
#include <oglplus/ext/EXT_direct_state_access.hpp>

#include <vector>
#include <sstream>
#include <iomanip>

class TextExample
 : public oglplus::StandaloneExample
{
private:
	oglplus::Context gl;
	oglplus::ARB_compatibility glc;
	oglplus::NV_path_rendering npr;
	oglplus::EXT_direct_state_access dsa;

	oglplus::PathArrayNV text_glyphs;
	std::vector<GLfloat> glyph_spacings;

	GLfloat font_y_min, font_y_max, font_height;
public:
	TextExample(int /*argc*/, const char** /*argv*/)
	 : text_glyphs(128)
	{
		using namespace oglplus;

		GLfloat font_scale = 48;

		text_glyphs.GlyphRange(
			PathNVFontTarget::Standard,
			"Sans",
			PathNVFontStyle::Bold,
			0, 128,
			PathNVMissingGlyph::Use,
			~0,
			font_scale
		);

		GLfloat color_gen_coeffs[9] = {
			-0.6f, 0.0f, 0.8f,
			 0.0f, 0.0f, 0.0f,
			 0.5f, 0.0f, 0.5f
		};
		npr.ColorGen(
			PathNVColor::Primary,
			PathNVGenMode::ObjectBoundingBox,
			PathNVColorFormat::RGB,
			color_gen_coeffs
		);

		gl.ClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		gl.ClearStencil(0);
		gl.StencilMask(~0);
		gl.StencilFunc(CompareFunction::NotEqual, 0, 0xFF);
		gl.StencilOp(
			StencilOperation::Keep,
			StencilOperation::Keep,
			StencilOperation::Zero
		);

		gl.Enable(Capability::StencilTest);
	}

	void RenderText(GLfloat x, GLfloat y, const oglplus::String& text)
	{
		using namespace oglplus;

		glyph_spacings.resize(text.size()+1);
		text_glyphs.GetSpacing(
			PathNVListMode::AccumAdjacentPairs,
			text,
			1.0f, 1.0f,
			PathNVTransformType::TranslateX,
			glyph_spacings
		);
		glyph_spacings.insert(glyph_spacings.begin(), 0);

		dsa.ModelviewMatrix()
			.LoadIdentity()
			.Translate(x, y, 0);

		text_glyphs.StencilFillInstanced(
			text,
			PathNVFillMode::CountUp,
			0xFF,
			PathNVTransformType::TranslateX,
			glyph_spacings
		);
		text_glyphs.CoverFillInstanced(
			text,
			PathNVFillCoverMode::BoundingBoxOfBoundingBoxes,
			PathNVTransformType::TranslateX,
			glyph_spacings
		);
	}

	void Render(void)
	{
		using namespace oglplus;

		gl.Clear().ColorBuffer().StencilBuffer();

		auto h = Height();
		float l = font_height / 2;
		int i = 1;
		RenderText(l, h - font_height*i++, "NV_path_rendering");

		{
			std::stringstream s;
			s << "Frame no.: " << FrameNumber();
			RenderText(l, h - font_height*i++, s.str());
		}
		{
			std::stringstream s;
			s << std::fixed << std::setprecision(2);
			s << "Time: " << FrameTime() << " [s]";
			RenderText(l, h - font_height*i++, s.str());
		}
		{
			std::stringstream s;
			s << std::fixed << std::setprecision(2);
			s << "Average FPS: " << (FrameTime()>0.0 ? AverageFPS() : 0.0);
			RenderText(l, h - font_height*i++, s.str());
		}
		{
			std::stringstream s;
			s << std::fixed << std::setprecision(2);
			s << "Current FPS: " << (FrameDuration()>0.0 ? CurrentFPS() : 0.0);
			RenderText(l, h - font_height*i++, s.str());
		}
		{
			std::stringstream s;
			s << "Viewport: " << Width() << " x " << Height();
			RenderText(l, h - font_height*i++, s.str());
		}
		{
			std::stringstream s;
			s << std::fixed << std::setprecision(3);
			s << "Aspect ratio: " << Aspect();
			RenderText(l, h - font_height*i++, s.str());
		}
	}

	void Reshape(void)
	{
		using namespace oglplus;

		gl.Viewport(Width(), Height());

		dsa.ProjectionMatrix()
			.LoadIdentity()
			.Ortho(0, Width(), 0, Height(), -1, 1);

		GLfloat font_min_max[2];
		text_glyphs.GetMetricRange(
			PathNVMetricQuery::FontYMinBounds|
			PathNVMetricQuery::FontYMaxBounds,
			1, 0,
			font_min_max
		);
		font_y_min = font_min_max[0];
		font_y_max = font_min_max[1];
		font_height = font_y_max - font_y_min;
	}
};

int main(int argc, char* argv[])
{
	return oglplus::GlutGlewMain<TextExample>(
		"Example of text rendering with NV_path_rendering",
		argc, argv
	);
}

