//  File implement/eglplus/enums/surface_attrib_names.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/surface_attrib.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	SurfaceAttrib*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_SURFACEATTRIB)
#define EGLPLUS_IMPL_EVN_SURFACEATTRIB
{
switch(value)
{
#if defined EGL_WIDTH
	case EGL_WIDTH: return StrCRef("WIDTH");
#endif
#if defined EGL_HEIGHT
	case EGL_HEIGHT: return StrCRef("HEIGHT");
#endif
#if defined EGL_CONFIG_ID
	case EGL_CONFIG_ID: return StrCRef("CONFIG_ID");
#endif
#if defined EGL_VG_ALPHA_FORMAT
	case EGL_VG_ALPHA_FORMAT: return StrCRef("VG_ALPHA_FORMAT");
#endif
#if defined EGL_VG_COLORSPACE
	case EGL_VG_COLORSPACE: return StrCRef("VG_COLORSPACE");
#endif
#if defined EGL_GL_COLORSPACE
	case EGL_GL_COLORSPACE: return StrCRef("GL_COLORSPACE");
#endif
#if defined EGL_HORIZONTAL_RESOLUTION
	case EGL_HORIZONTAL_RESOLUTION: return StrCRef("HORIZONTAL_RESOLUTION");
#endif
#if defined EGL_VERTICAL_RESOLUTION
	case EGL_VERTICAL_RESOLUTION: return StrCRef("VERTICAL_RESOLUTION");
#endif
#if defined EGL_LARGEST_PBUFFER
	case EGL_LARGEST_PBUFFER: return StrCRef("LARGEST_PBUFFER");
#endif
#if defined EGL_MIPMAP_TEXTURE
	case EGL_MIPMAP_TEXTURE: return StrCRef("MIPMAP_TEXTURE");
#endif
#if defined EGL_MIPMAP_LEVEL
	case EGL_MIPMAP_LEVEL: return StrCRef("MIPMAP_LEVEL");
#endif
#if defined EGL_MULTISAMPLE_RESOLVE
	case EGL_MULTISAMPLE_RESOLVE: return StrCRef("MULTISAMPLE_RESOLVE");
#endif
#if defined EGL_PIXEL_ASPECT_RATIO
	case EGL_PIXEL_ASPECT_RATIO: return StrCRef("PIXEL_ASPECT_RATIO");
#endif
#if defined EGL_RENDER_BUFFER
	case EGL_RENDER_BUFFER: return StrCRef("RENDER_BUFFER");
#endif
#if defined EGL_SWAP_BEHAVIOR
	case EGL_SWAP_BEHAVIOR: return StrCRef("SWAP_BEHAVIOR");
#endif
#if defined EGL_TEXTURE_FORMAT
	case EGL_TEXTURE_FORMAT: return StrCRef("TEXTURE_FORMAT");
#endif
#if defined EGL_TEXTURE_TARGET
	case EGL_TEXTURE_TARGET: return StrCRef("TEXTURE_TARGET");
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

