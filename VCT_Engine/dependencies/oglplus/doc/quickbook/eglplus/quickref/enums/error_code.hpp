//  File doc/quickbook/eglplus/quickref/enums/error_code.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/error_code.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_error_code

enum class ErrorCode : EGLenum
{
	NotInitialized    = EGL_NOT_INITIALIZED,
	BadAccess         = EGL_BAD_ACCESS,
	BadAlloc          = EGL_BAD_ALLOC,
	BadAttribute      = EGL_BAD_ATTRIBUTE,
	BadContext        = EGL_BAD_CONTEXT,
	BadConfig         = EGL_BAD_CONFIG,
	BadCurrentSurface = EGL_BAD_CURRENT_SURFACE,
	BadDisplay        = EGL_BAD_DISPLAY,
	BadSurface        = EGL_BAD_SURFACE,
	BadMatch          = EGL_BAD_MATCH,
	BadParameter      = EGL_BAD_PARAMETER,
	BadNativePixmap   = EGL_BAD_NATIVE_PIXMAP,
	BadNativeWindow   = EGL_BAD_NATIVE_WINDOW,
	ContextLost       = EGL_CONTEXT_LOST
};

template <>
__Range<ErrorCode> __EnumValueRange<ErrorCode>(void);

__StrCRef __EnumValueName(ErrorCode);

//]
