//  File implement/eglplus/enums/context_attrib_names.ipp
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
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	ContextAttrib*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_CONTEXTATTRIB)
#define EGLPLUS_IMPL_EVN_CONTEXTATTRIB
{
switch(value)
{
#if defined EGL_CONTEXT_MAJOR_VERSION
	case EGL_CONTEXT_MAJOR_VERSION: return StrCRef("CONTEXT_MAJOR_VERSION");
#endif
#if defined EGL_CONTEXT_MINOR_VERSION
	case EGL_CONTEXT_MINOR_VERSION: return StrCRef("CONTEXT_MINOR_VERSION");
#endif
#if defined EGL_CONTEXT_FLAGS
	case EGL_CONTEXT_FLAGS: return StrCRef("CONTEXT_FLAGS");
#endif
#if defined EGL_CONTEXT_OPENGL_PROFILE_MASK
	case EGL_CONTEXT_OPENGL_PROFILE_MASK: return StrCRef("CONTEXT_OPENGL_PROFILE_MASK");
#endif
#if defined EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY
	case EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY: return StrCRef("CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY");
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

