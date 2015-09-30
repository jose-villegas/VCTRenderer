//  File doc/quickbook/eglplus/quickref/enums/context_flag.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/context_flag.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_context_flag

enum class ContextFlag : EGLenum
{
	OpenGLDebug             = EGL_CONTEXT_OPENGL_DEBUG_BIT,
	OpenGLForwardCompatible = EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT,
	OpenGLRobustAccess      = EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT
};

template <>
__Range<ContextFlag> __EnumValueRange<ContextFlag>(void);

__StrCRef __EnumValueName(ContextFlag);

//]
