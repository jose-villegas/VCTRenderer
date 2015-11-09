//  File implement/eglplus/enums/gl_colorspace_names.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/gl_colorspace.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	GLColorspace*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_GLCOLORSPACE)
#define EGLPLUS_IMPL_EVN_GLCOLORSPACE
{
switch(value)
{
#if defined EGL_GL_COLORSPACE_sRGB
	case EGL_GL_COLORSPACE_sRGB: return StrCRef("GL_COLORSPACE_sRGB");
#endif
#if defined EGL_GL_COLORSPACE_LINEAR
	case EGL_GL_COLORSPACE_LINEAR: return StrCRef("GL_COLORSPACE_LINEAR");
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

