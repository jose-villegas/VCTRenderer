//  File implement/eglplus/enums/surface_attrib_range.ipp
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
EGLPLUS_LIB_FUNC aux::CastIterRange<
	const EGLenum*,
	SurfaceAttrib
> ValueRange_(SurfaceAttrib*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_SURFACEATTRIB)
#define EGLPLUS_IMPL_EVR_SURFACEATTRIB
{
static const EGLenum _values[] = {
#if defined EGL_WIDTH
EGL_WIDTH,
#endif
#if defined EGL_HEIGHT
EGL_HEIGHT,
#endif
#if defined EGL_CONFIG_ID
EGL_CONFIG_ID,
#endif
#if defined EGL_VG_ALPHA_FORMAT
EGL_VG_ALPHA_FORMAT,
#endif
#if defined EGL_VG_COLORSPACE
EGL_VG_COLORSPACE,
#endif
#if defined EGL_GL_COLORSPACE
EGL_GL_COLORSPACE,
#endif
#if defined EGL_HORIZONTAL_RESOLUTION
EGL_HORIZONTAL_RESOLUTION,
#endif
#if defined EGL_VERTICAL_RESOLUTION
EGL_VERTICAL_RESOLUTION,
#endif
#if defined EGL_LARGEST_PBUFFER
EGL_LARGEST_PBUFFER,
#endif
#if defined EGL_MIPMAP_TEXTURE
EGL_MIPMAP_TEXTURE,
#endif
#if defined EGL_MIPMAP_LEVEL
EGL_MIPMAP_LEVEL,
#endif
#if defined EGL_MULTISAMPLE_RESOLVE
EGL_MULTISAMPLE_RESOLVE,
#endif
#if defined EGL_PIXEL_ASPECT_RATIO
EGL_PIXEL_ASPECT_RATIO,
#endif
#if defined EGL_RENDER_BUFFER
EGL_RENDER_BUFFER,
#endif
#if defined EGL_SWAP_BEHAVIOR
EGL_SWAP_BEHAVIOR,
#endif
#if defined EGL_TEXTURE_FORMAT
EGL_TEXTURE_FORMAT,
#endif
#if defined EGL_TEXTURE_TARGET
EGL_TEXTURE_TARGET,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	SurfaceAttrib
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

