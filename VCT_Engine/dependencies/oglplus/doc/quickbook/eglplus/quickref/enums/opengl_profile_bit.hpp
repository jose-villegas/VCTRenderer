//  File doc/quickbook/eglplus/quickref/enums/opengl_profile_bit.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/opengl_profile_bit.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_opengl_profile_bit

enum class OpenGLProfileBit : EGLenum
{
	Core          = EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
	Compatibility = EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT
};

template <>
__Range<OpenGLProfileBit> __EnumValueRange<OpenGLProfileBit>(void);

__StrCRef __EnumValueName(OpenGLProfileBit);

//]
