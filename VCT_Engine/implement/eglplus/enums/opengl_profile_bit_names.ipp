//  File implement/eglplus/enums/opengl_profile_bit_names.ipp
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
namespace enums {
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	OpenGLProfileBit*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_OPENGLPROFILEBIT)
#define EGLPLUS_IMPL_EVN_OPENGLPROFILEBIT
{
switch(value)
{
#if defined EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT
	case EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT: return StrCRef("CONTEXT_OPENGL_CORE_PROFILE_BIT");
#endif
#if defined EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT
	case EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT: return StrCRef("CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT");
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

