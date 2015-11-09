//  File implement/eglplus/enums/context_attrib_range.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/context_attrib.txt'
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
	ContextAttrib
> ValueRange_(ContextAttrib*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_CONTEXTATTRIB)
#define EGLPLUS_IMPL_EVR_CONTEXTATTRIB
{
static const EGLenum _values[] = {
#if defined EGL_CONTEXT_MAJOR_VERSION
EGL_CONTEXT_MAJOR_VERSION,
#endif
#if defined EGL_CONTEXT_MINOR_VERSION
EGL_CONTEXT_MINOR_VERSION,
#endif
#if defined EGL_CONTEXT_FLAGS
EGL_CONTEXT_FLAGS,
#endif
#if defined EGL_CONTEXT_OPENGL_PROFILE_MASK
EGL_CONTEXT_OPENGL_PROFILE_MASK,
#endif
#if defined EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY
EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	ContextAttrib
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

