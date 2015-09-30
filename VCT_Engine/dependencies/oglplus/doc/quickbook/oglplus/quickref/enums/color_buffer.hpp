//  File doc/quickbook/oglplus/quickref/enums/color_buffer.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/color_buffer.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_color_buffer

enum class ColorBuffer : GLenum
{
	None         = GL_NONE,
	FrontLeft    = GL_FRONT_LEFT,
	FrontRight   = GL_FRONT_RIGHT,
	BackLeft     = GL_BACK_LEFT,
	BackRight    = GL_BACK_RIGHT,
	Front        = GL_FRONT,
	Back         = GL_BACK,
	Left         = GL_LEFT,
	Right        = GL_RIGHT,
	FrontAndBack = GL_FRONT_AND_BACK
};

template <>
__Range<ColorBuffer> __EnumValueRange<ColorBuffer>(void);

__StrCRef __EnumValueName(ColorBuffer);

//]
