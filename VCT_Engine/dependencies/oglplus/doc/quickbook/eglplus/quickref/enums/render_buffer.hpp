//  File doc/quickbook/eglplus/quickref/enums/render_buffer.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/render_buffer.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_render_buffer

enum class RenderBuffer : EGLenum
{
	SingleBuffer = EGL_SINGLE_BUFFER,
	BackBuffer   = EGL_BACK_BUFFER
};

template <>
__Range<RenderBuffer> __EnumValueRange<RenderBuffer>(void);

__StrCRef __EnumValueName(RenderBuffer);

//]
