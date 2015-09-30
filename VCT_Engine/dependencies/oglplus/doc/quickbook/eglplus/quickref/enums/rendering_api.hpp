//  File doc/quickbook/eglplus/quickref/enums/rendering_api.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/rendering_api.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_rendering_api

enum class RenderingAPI : EGLenum
{
	OpenGL    = EGL_OPENGL_API,
	OpenGL_ES = EGL_OPENGL_ES_API,
	OpenVG    = EGL_OPENVG_API,
	None      = EGL_NONE
};

template <>
__Range<RenderingAPI> __EnumValueRange<RenderingAPI>(void);

__StrCRef __EnumValueName(RenderingAPI);

//]
