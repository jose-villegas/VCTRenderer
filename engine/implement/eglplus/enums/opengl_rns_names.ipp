//  File implement/eglplus/enums/opengl_rns_names.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/opengl_rns.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	OpenGLResetNotificationStrategy*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_OPENGLRESETNOTIFICATIONSTRATEGY)
#define EGLPLUS_IMPL_EVN_OPENGLRESETNOTIFICATIONSTRATEGY
{
switch(value)
{
#if defined EGL_NO_RESET_NOTIFICATION
	case EGL_NO_RESET_NOTIFICATION: return StrCRef("NO_RESET_NOTIFICATION");
#endif
#if defined EGL_LOSE_CONTEXT_ON_RESET
	case EGL_LOSE_CONTEXT_ON_RESET: return StrCRef("LOSE_CONTEXT_ON_RESET");
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

