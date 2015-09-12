//  File implement/eglplus/enums/error_code_range.ipp
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
EGLPLUS_LIB_FUNC aux::CastIterRange<
	const EGLenum*,
	ErrorCode
> ValueRange_(ErrorCode*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_ERRORCODE)
#define EGLPLUS_IMPL_EVR_ERRORCODE
{
static const EGLenum _values[] = {
#if defined EGL_NOT_INITIALIZED
EGL_NOT_INITIALIZED,
#endif
#if defined EGL_BAD_ACCESS
EGL_BAD_ACCESS,
#endif
#if defined EGL_BAD_ALLOC
EGL_BAD_ALLOC,
#endif
#if defined EGL_BAD_ATTRIBUTE
EGL_BAD_ATTRIBUTE,
#endif
#if defined EGL_BAD_CONTEXT
EGL_BAD_CONTEXT,
#endif
#if defined EGL_BAD_CONFIG
EGL_BAD_CONFIG,
#endif
#if defined EGL_BAD_CURRENT_SURFACE
EGL_BAD_CURRENT_SURFACE,
#endif
#if defined EGL_BAD_DISPLAY
EGL_BAD_DISPLAY,
#endif
#if defined EGL_BAD_SURFACE
EGL_BAD_SURFACE,
#endif
#if defined EGL_BAD_MATCH
EGL_BAD_MATCH,
#endif
#if defined EGL_BAD_PARAMETER
EGL_BAD_PARAMETER,
#endif
#if defined EGL_BAD_NATIVE_PIXMAP
EGL_BAD_NATIVE_PIXMAP,
#endif
#if defined EGL_BAD_NATIVE_WINDOW
EGL_BAD_NATIVE_WINDOW,
#endif
#if defined EGL_CONTEXT_LOST
EGL_CONTEXT_LOST,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	ErrorCode
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

