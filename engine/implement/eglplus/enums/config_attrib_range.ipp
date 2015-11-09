//  File implement/eglplus/enums/config_attrib_range.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/config_attrib.txt'
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
	ConfigAttrib
> ValueRange_(ConfigAttrib*)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVR_CONFIGATTRIB)
#define EGLPLUS_IMPL_EVR_CONFIGATTRIB
{
static const EGLenum _values[] = {
#if defined EGL_BUFFER_SIZE
EGL_BUFFER_SIZE,
#endif
#if defined EGL_RED_SIZE
EGL_RED_SIZE,
#endif
#if defined EGL_GREEN_SIZE
EGL_GREEN_SIZE,
#endif
#if defined EGL_BLUE_SIZE
EGL_BLUE_SIZE,
#endif
#if defined EGL_LUMINANCE_SIZE
EGL_LUMINANCE_SIZE,
#endif
#if defined EGL_ALPHA_SIZE
EGL_ALPHA_SIZE,
#endif
#if defined EGL_ALPHA_MASK_SIZE
EGL_ALPHA_MASK_SIZE,
#endif
#if defined EGL_BIND_TO_TEXTURE_RGB
EGL_BIND_TO_TEXTURE_RGB,
#endif
#if defined EGL_BIND_TO_TEXTURE_RGBA
EGL_BIND_TO_TEXTURE_RGBA,
#endif
#if defined EGL_COLOR_BUFFER_TYPE
EGL_COLOR_BUFFER_TYPE,
#endif
#if defined EGL_CONFIG_CAVEAT
EGL_CONFIG_CAVEAT,
#endif
#if defined EGL_CONFIG_ID
EGL_CONFIG_ID,
#endif
#if defined EGL_CONFORMANT
EGL_CONFORMANT,
#endif
#if defined EGL_DEPTH_SIZE
EGL_DEPTH_SIZE,
#endif
#if defined EGL_LEVEL
EGL_LEVEL,
#endif
#if defined EGL_MAX_PBUFFER_WIDTH
EGL_MAX_PBUFFER_WIDTH,
#endif
#if defined EGL_MAX_PBUFFER_HEIGHT
EGL_MAX_PBUFFER_HEIGHT,
#endif
#if defined EGL_MAX_PBUFFER_PIXELS
EGL_MAX_PBUFFER_PIXELS,
#endif
#if defined EGL_MAX_SWAP_INTERVAL
EGL_MAX_SWAP_INTERVAL,
#endif
#if defined EGL_MIN_SWAP_INTERVAL
EGL_MIN_SWAP_INTERVAL,
#endif
#if defined EGL_NATIVE_RENDERABLE
EGL_NATIVE_RENDERABLE,
#endif
#if defined EGL_NATIVE_VISUAL_ID
EGL_NATIVE_VISUAL_ID,
#endif
#if defined EGL_NATIVE_VISUAL_TYPE
EGL_NATIVE_VISUAL_TYPE,
#endif
#if defined EGL_RENDERABLE_TYPE
EGL_RENDERABLE_TYPE,
#endif
#if defined EGL_SAMPLE_BUFFERS
EGL_SAMPLE_BUFFERS,
#endif
#if defined EGL_SAMPLES
EGL_SAMPLES,
#endif
#if defined EGL_STENCIL_SIZE
EGL_STENCIL_SIZE,
#endif
#if defined EGL_SURFACE_TYPE
EGL_SURFACE_TYPE,
#endif
#if defined EGL_TRANSPARENT_TYPE
EGL_TRANSPARENT_TYPE,
#endif
#if defined EGL_TRANSPARENT_RED_VALUE
EGL_TRANSPARENT_RED_VALUE,
#endif
#if defined EGL_TRANSPARENT_GREEN_VALUE
EGL_TRANSPARENT_GREEN_VALUE,
#endif
#if defined EGL_TRANSPARENT_BLUE_VALUE
EGL_TRANSPARENT_BLUE_VALUE,
#endif
0
};
return aux::CastIterRange<
	const EGLenum*,
	ConfigAttrib
>(_values, _values+sizeof(_values)/sizeof(_values[0])-1);
}
#else
;
#endif
} // namespace enums

