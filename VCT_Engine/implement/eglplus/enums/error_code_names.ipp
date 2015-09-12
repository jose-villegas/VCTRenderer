//  File implement/eglplus/enums/error_code_names.ipp
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
namespace enums {
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	ErrorCode*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_ERRORCODE)
#define EGLPLUS_IMPL_EVN_ERRORCODE
{
switch(value)
{
#if defined EGL_NOT_INITIALIZED
	case EGL_NOT_INITIALIZED: return StrCRef("NOT_INITIALIZED");
#endif
#if defined EGL_BAD_ACCESS
	case EGL_BAD_ACCESS: return StrCRef("BAD_ACCESS");
#endif
#if defined EGL_BAD_ALLOC
	case EGL_BAD_ALLOC: return StrCRef("BAD_ALLOC");
#endif
#if defined EGL_BAD_ATTRIBUTE
	case EGL_BAD_ATTRIBUTE: return StrCRef("BAD_ATTRIBUTE");
#endif
#if defined EGL_BAD_CONTEXT
	case EGL_BAD_CONTEXT: return StrCRef("BAD_CONTEXT");
#endif
#if defined EGL_BAD_CONFIG
	case EGL_BAD_CONFIG: return StrCRef("BAD_CONFIG");
#endif
#if defined EGL_BAD_CURRENT_SURFACE
	case EGL_BAD_CURRENT_SURFACE: return StrCRef("BAD_CURRENT_SURFACE");
#endif
#if defined EGL_BAD_DISPLAY
	case EGL_BAD_DISPLAY: return StrCRef("BAD_DISPLAY");
#endif
#if defined EGL_BAD_SURFACE
	case EGL_BAD_SURFACE: return StrCRef("BAD_SURFACE");
#endif
#if defined EGL_BAD_MATCH
	case EGL_BAD_MATCH: return StrCRef("BAD_MATCH");
#endif
#if defined EGL_BAD_PARAMETER
	case EGL_BAD_PARAMETER: return StrCRef("BAD_PARAMETER");
#endif
#if defined EGL_BAD_NATIVE_PIXMAP
	case EGL_BAD_NATIVE_PIXMAP: return StrCRef("BAD_NATIVE_PIXMAP");
#endif
#if defined EGL_BAD_NATIVE_WINDOW
	case EGL_BAD_NATIVE_WINDOW: return StrCRef("BAD_NATIVE_WINDOW");
#endif
#if defined EGL_CONTEXT_LOST
	case EGL_CONTEXT_LOST: return StrCRef("CONTEXT_LOST");
#endif
	default:;
}
EGLPLUS_FAKE_USE(value);
return StrCRef();
}
#else
;
#endif
} // namespace enums

