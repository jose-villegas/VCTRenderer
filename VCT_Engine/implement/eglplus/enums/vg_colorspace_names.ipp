//  File implement/eglplus/enums/vg_colorspace_names.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/vg_colorspace.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	VGColorspace*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_VGCOLORSPACE)
#define EGLPLUS_IMPL_EVN_VGCOLORSPACE
{
switch(value)
{
#if defined EGL_VG_COLORSPACE_sRGB
	case EGL_VG_COLORSPACE_sRGB: return StrCRef("VG_COLORSPACE_sRGB");
#endif
#if defined EGL_VG_COLORSPACE_LINEAR
	case EGL_VG_COLORSPACE_LINEAR: return StrCRef("VG_COLORSPACE_LINEAR");
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

