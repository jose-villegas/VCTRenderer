//  File implement/eglplus/enums/context_flag_range.ipp
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
namespace enums {
EGLPLUS_LIB_FUNC aux::CastIterRange<
	const EGLenum*,
	ContextFlag
> ValueRange_(ContextFlag*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_CONTEXTFLAG)
#define EGLPLUS_IMPL_EVR_CONTEXTFLAG
{
static const EGLenum _values[] = {
#if defined EGL_CONTEXT_OPENGL_DEBUG_BIT
EGL_CONTEXT_OPENGL_DEBUG_BIT,
#endif
#if defined EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT
EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT,
#endif
#if defined EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT
EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	ContextFlag
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

