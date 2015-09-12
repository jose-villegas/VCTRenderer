//  File implement/eglplus/enums/config_attrib_names.ipp
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
EGLPLUS_LIB_FUNC StrCRef ValueName_(
	ConfigAttrib*,
	EGLenum value
)
#if (!EGLPLUS_LINK_LIBRARY || defined(EGLPLUS_IMPLEMENTING_LIBRARY)) && \
	!defined(EGLPLUS_IMPL_EVN_CONFIGATTRIB)
#define EGLPLUS_IMPL_EVN_CONFIGATTRIB
{
switch(value)
{
#if defined EGL_BUFFER_SIZE
	case EGL_BUFFER_SIZE: return StrCRef("BUFFER_SIZE");
#endif
#if defined EGL_RED_SIZE
	case EGL_RED_SIZE: return StrCRef("RED_SIZE");
#endif
#if defined EGL_GREEN_SIZE
	case EGL_GREEN_SIZE: return StrCRef("GREEN_SIZE");
#endif
#if defined EGL_BLUE_SIZE
	case EGL_BLUE_SIZE: return StrCRef("BLUE_SIZE");
#endif
#if defined EGL_LUMINANCE_SIZE
	case EGL_LUMINANCE_SIZE: return StrCRef("LUMINANCE_SIZE");
#endif
#if defined EGL_ALPHA_SIZE
	case EGL_ALPHA_SIZE: return StrCRef("ALPHA_SIZE");
#endif
#if defined EGL_ALPHA_MASK_SIZE
	case EGL_ALPHA_MASK_SIZE: return StrCRef("ALPHA_MASK_SIZE");
#endif
#if defined EGL_BIND_TO_TEXTURE_RGB
	case EGL_BIND_TO_TEXTURE_RGB: return StrCRef("BIND_TO_TEXTURE_RGB");
#endif
#if defined EGL_BIND_TO_TEXTURE_RGBA
	case EGL_BIND_TO_TEXTURE_RGBA: return StrCRef("BIND_TO_TEXTURE_RGBA");
#endif
#if defined EGL_COLOR_BUFFER_TYPE
	case EGL_COLOR_BUFFER_TYPE: return StrCRef("COLOR_BUFFER_TYPE");
#endif
#if defined EGL_CONFIG_CAVEAT
	case EGL_CONFIG_CAVEAT: return StrCRef("CONFIG_CAVEAT");
#endif
#if defined EGL_CONFIG_ID
	case EGL_CONFIG_ID: return StrCRef("CONFIG_ID");
#endif
#if defined EGL_CONFORMANT
	case EGL_CONFORMANT: return StrCRef("CONFORMANT");
#endif
#if defined EGL_DEPTH_SIZE
	case EGL_DEPTH_SIZE: return StrCRef("DEPTH_SIZE");
#endif
#if defined EGL_LEVEL
	case EGL_LEVEL: return StrCRef("LEVEL");
#endif
#if defined EGL_MAX_PBUFFER_WIDTH
	case EGL_MAX_PBUFFER_WIDTH: return StrCRef("MAX_PBUFFER_WIDTH");
#endif
#if defined EGL_MAX_PBUFFER_HEIGHT
	case EGL_MAX_PBUFFER_HEIGHT: return StrCRef("MAX_PBUFFER_HEIGHT");
#endif
#if defined EGL_MAX_PBUFFER_PIXELS
	case EGL_MAX_PBUFFER_PIXELS: return StrCRef("MAX_PBUFFER_PIXELS");
#endif
#if defined EGL_MAX_SWAP_INTERVAL
	case EGL_MAX_SWAP_INTERVAL: return StrCRef("MAX_SWAP_INTERVAL");
#endif
#if defined EGL_MIN_SWAP_INTERVAL
	case EGL_MIN_SWAP_INTERVAL: return StrCRef("MIN_SWAP_INTERVAL");
#endif
#if defined EGL_NATIVE_RENDERABLE
	case EGL_NATIVE_RENDERABLE: return StrCRef("NATIVE_RENDERABLE");
#endif
#if defined EGL_NATIVE_VISUAL_ID
	case EGL_NATIVE_VISUAL_ID: return StrCRef("NATIVE_VISUAL_ID");
#endif
#if defined EGL_NATIVE_VISUAL_TYPE
	case EGL_NATIVE_VISUAL_TYPE: return StrCRef("NATIVE_VISUAL_TYPE");
#endif
#if defined EGL_RENDERABLE_TYPE
	case EGL_RENDERABLE_TYPE: return StrCRef("RENDERABLE_TYPE");
#endif
#if defined EGL_SAMPLE_BUFFERS
	case EGL_SAMPLE_BUFFERS: return StrCRef("SAMPLE_BUFFERS");
#endif
#if defined EGL_SAMPLES
	case EGL_SAMPLES: return StrCRef("SAMPLES");
#endif
#if defined EGL_STENCIL_SIZE
	case EGL_STENCIL_SIZE: return StrCRef("STENCIL_SIZE");
#endif
#if defined EGL_SURFACE_TYPE
	case EGL_SURFACE_TYPE: return StrCRef("SURFACE_TYPE");
#endif
#if defined EGL_TRANSPARENT_TYPE
	case EGL_TRANSPARENT_TYPE: return StrCRef("TRANSPARENT_TYPE");
#endif
#if defined EGL_TRANSPARENT_RED_VALUE
	case EGL_TRANSPARENT_RED_VALUE: return StrCRef("TRANSPARENT_RED_VALUE");
#endif
#if defined EGL_TRANSPARENT_GREEN_VALUE
	case EGL_TRANSPARENT_GREEN_VALUE: return StrCRef("TRANSPARENT_GREEN_VALUE");
#endif
#if defined EGL_TRANSPARENT_BLUE_VALUE
	case EGL_TRANSPARENT_BLUE_VALUE: return StrCRef("TRANSPARENT_BLUE_VALUE");
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

