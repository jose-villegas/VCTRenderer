//  File doc/quickbook/eglplus/quickref/enums/color_buffer_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/color_buffer_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_color_buffer_type

enum class ColorBufferType : EGLenum
{
	RGBBuffer       = EGL_RGB_BUFFER,
	LuminanceBuffer = EGL_LUMINANCE_BUFFER
};

template <>
__Range<ColorBufferType> __EnumValueRange<ColorBufferType>(void);

__StrCRef __EnumValueName(ColorBufferType);

//]
