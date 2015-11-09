//  File implement/eglplus/enums/context_flag_names.ipp
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
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	ContextFlag*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_CONTEXTFLAG)
#define EGLPLUS_IMPL_EVN_CONTEXTFLAG
{
switch(value)
{
#if defined EGL_CONTEXT_OPENGL_DEBUG_BIT
	case EGL_CONTEXT_OPENGL_DEBUG_BIT: return StrCRef("CONTEXT_OPENGL_DEBUG_BIT");
#endif
#if defined EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT
	case EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT: return StrCRef("CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT");
#endif
#if defined EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT
	case EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT: return StrCRef("CONTEXT_OPENGL_ROBUST_ACCESS_BIT");
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

