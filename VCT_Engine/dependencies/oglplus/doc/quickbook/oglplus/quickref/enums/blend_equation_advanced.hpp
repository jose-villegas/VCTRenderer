//  File doc/quickbook/oglplus/quickref/enums/blend_equation_advanced.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/blend_equation_advanced.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_blend_equation_advanced

enum class BlendEquationAdvanced : GLenum
{
	Multiply      = GL_MULTIPLY_KHR,
	Screen        = GL_SCREEN_KHR,
	Overlay       = GL_OVERLAY_KHR,
	Darken        = GL_DARKEN_KHR,
	Lighten       = GL_LIGHTEN_KHR,
	Colordodge    = GL_COLORDODGE_KHR,
	Colorburn     = GL_COLORBURN_KHR,
	Hardlight     = GL_HARDLIGHT_KHR,
	Softlight     = GL_SOFTLIGHT_KHR,
	Difference    = GL_DIFFERENCE_KHR,
	Exclusion     = GL_EXCLUSION_KHR,
	HSLHue        = GL_HSL_HUE_KHR,
	HSLSaturation = GL_HSL_SATURATION_KHR,
	HSLColor      = GL_HSL_COLOR_KHR,
	HSLLuminosity = GL_HSL_LUMINOSITY_KHR
};

template <>
__Range<BlendEquationAdvanced> __EnumValueRange<BlendEquationAdvanced>(void);

__StrCRef __EnumValueName(BlendEquationAdvanced);

//]
