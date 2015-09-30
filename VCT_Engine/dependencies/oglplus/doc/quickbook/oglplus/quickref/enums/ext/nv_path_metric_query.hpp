//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_metric_query.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_metric_query.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_metric_query

enum class PathNVMetricQuery : GLbitfield
{
	GlyphWidth                    = GL_GLYPH_WIDTH_BIT_NV,
	GlyphHeight                   = GL_GLYPH_HEIGHT_BIT_NV,
	GlyphHorizontalBearingX       = GL_GLYPH_HORIZONTAL_BEARING_X_BIT_NV,
	GlyphHorizontalBearingY       = GL_GLYPH_HORIZONTAL_BEARING_Y_BIT_NV,
	GlyphHorizontalBearingAdvance = GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV,
	GlyphVerticalBearingX         = GL_GLYPH_VERTICAL_BEARING_X_BIT_NV,
	GlyphVerticalBearingY         = GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV,
	GlyphVerticalBearingAdvance   = GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV,
	GlyphHasKerning               = GL_GLYPH_HAS_KERNING_BIT_NV,
	FontXMinBounds                = GL_FONT_X_MIN_BOUNDS_BIT_NV,
	FontYMinBounds                = GL_FONT_Y_MIN_BOUNDS_BIT_NV,
	FontXMaxBounds                = GL_FONT_X_MAX_BOUNDS_BIT_NV,
	FontYMaxBounds                = GL_FONT_Y_MAX_BOUNDS_BIT_NV,
	FontUnitsPerEm                = GL_FONT_UNITS_PER_EM_BIT_NV,
	FontAscender                  = GL_FONT_ASCENDER_BIT_NV,
	FontDescender                 = GL_FONT_DESCENDER_BIT_NV,
	FontHeight                    = GL_FONT_HEIGHT_BIT_NV,
	FontMaxAdvanceWidth           = GL_FONT_MAX_ADVANCE_WIDTH_BIT_NV,
	FontMaxAdvanceHeight          = GL_FONT_MAX_ADVANCE_HEIGHT_BIT_NV,
	FontUnderlinePosition         = GL_FONT_UNDERLINE_POSITION_BIT_NV,
	FontUnderlineThickness        = GL_FONT_UNDERLINE_THICKNESS_BIT_NV,
	FontHasKerning                = GL_FONT_HAS_KERNING_BIT_NV
};

template <>
__Range<PathNVMetricQuery> __EnumValueRange<PathNVMetricQuery>(void);

__StrCRef __EnumValueName(PathNVMetricQuery);

__Bitfield<PathNVMetricQuery> operator | (PathNVMetricQuery b1, PathNVMetricQuery b2);

//]
