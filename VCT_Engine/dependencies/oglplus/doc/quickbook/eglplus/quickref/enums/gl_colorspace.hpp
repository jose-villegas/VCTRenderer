//  File doc/quickbook/eglplus/quickref/enums/gl_colorspace.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/gl_colorspace.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_gl_colorspace

enum class GLColorspace : EGLenum
{
	sRGB   = EGL_GL_COLORSPACE_sRGB,
	Linear = EGL_GL_COLORSPACE_LINEAR
};

template <>
__Range<GLColorspace> __EnumValueRange<GLColorspace>(void);

__StrCRef __EnumValueName(GLColorspace);

//]
