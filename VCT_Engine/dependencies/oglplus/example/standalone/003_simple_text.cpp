/**
 *  @example standalone/003_simple_text.cpp
 *  @brief Shows usage of the NV_path_rendering extension for rendering text
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "glut_glew_example.hpp"

#include <oglplus/all.hpp>
#include <oglplus/ext/ARB_compatibility.hpp>
#include <oglplus/ext/NV_path_rendering.hpp>
#include <oglplus/ext/EXT_direct_state_access.hpp>
#include <oglplus/ext/EXT_direct_state_access/matrix.hpp>

#include <vector>

class TextExample
 : public oglplus::StandaloneExample
{
private:
	oglplus::Context gl;
	oglplus::ARB_compatibility glc;
	oglplus::NV_path_rendering pr;
	oglplus::EXT_direct_state_access dsa;
	oglplus::DSAModelviewMatrixEXT modelview;
	oglplus::DSAProjectionMatrixEXT projection;

	const oglplus::String text;
	oglplus::PathArrayNV text_path;
	std::vector<GLubyte> glyph_indices;

	static std::vector<GLubyte> make_glyph_indices(void)
	{
		const GLubyte indices[7] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x00};
		return std::vector<GLubyte>(indices, indices+7);
	}

	std::vector<GLfloat> glyph_spacings;
public:
	TextExample(int /*argc*/, const char** /*argv*/)
	 : text("OpenGL")
	 , text_path(text.size())
	 , glyph_indices(make_glyph_indices())
	 , glyph_spacings(glyph_indices.size())
	{
		using namespace oglplus;

		GLfloat font_scale = 64;

		text_path.Glyphs(
			PathNVFontTarget::Standard,
			"Sans",
			PathNVFontStyle::Bold,
			text,
			PathNVMissingGlyph::Use,
			~0,
			font_scale
		);

		text_path.GetSpacing(
			PathNVListMode::AccumAdjacentPairs,
			glyph_indices,
			1.0f, 1.0f,
			PathNVTransformType::TranslateX,
			glyph_spacings
		);

		glyph_spacings.insert(glyph_spacings.begin(), 0);
		glyph_spacings.pop_back();
		glyph_indices.pop_back();

		GLfloat text_left = glyph_spacings.front();
		GLfloat text_right = glyph_spacings.back();

		GLfloat font_min_max[2];

		text_path.GetMetricRange(
			PathNVMetricQuery::FontYMinBounds|
			PathNVMetricQuery::FontYMaxBounds,
			1, 0,
			font_min_max
		);

		projection.Ortho(
			text_left -10,
			text_right+10,
			font_min_max[0],
			font_min_max[1],
			-1.0,
			+1.0
		);
		modelview.LoadIdentity();

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

	void Reshape(void)
	{
		using namespace oglplus;

		gl.Viewport(Width(), Height());
	}

	void Render(void)
	{
		using namespace oglplus;

		gl.Clear().ColorBuffer().StencilBuffer();

		glc.Color(0.2, 0.2, 1.0);

		text_path.StencilFillInstanced(
			glyph_indices,
			PathNVFillMode::CountUp,
			0xFF,
			PathNVTransformType::TranslateX,
			glyph_spacings
		);
		text_path.CoverFillInstanced(
			glyph_indices,
			PathNVFillCoverMode::BoundingBoxOfBoundingBoxes,
			PathNVTransformType::TranslateX,
			glyph_spacings
		);

		glc.Color(0.0, 0.0, 0.0);

		text_path.StencilStrokeInstanced(
			glyph_indices,
			1, ~0,
			PathNVTransformType::TranslateX,
			glyph_spacings
		);
		text_path.CoverStrokeInstanced(
			glyph_indices,
			PathNVStrokeCoverMode::ConvexHull,
			PathNVTransformType::TranslateX,
			glyph_spacings
		);
	}
};

int main(int argc, char* argv[])
{
	return oglplus::GlutGlewMain<TextExample>(
		"Example of text rendering with NV_path_rendering",
		argc, argv
	);
}

