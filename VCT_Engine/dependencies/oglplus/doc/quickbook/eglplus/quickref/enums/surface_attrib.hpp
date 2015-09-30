//  File doc/quickbook/eglplus/quickref/enums/surface_attrib.hpp
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
//[eglplus_enums_surface_attrib

enum class SurfaceAttrib : EGLenum
{
	Width                = EGL_WIDTH,
	Height               = EGL_HEIGHT,
	ConfigId             = EGL_CONFIG_ID,
	VGAlphaFormat        = EGL_VG_ALPHA_FORMAT,
	VGColorspace         = EGL_VG_COLORSPACE,
	GLColorspace         = EGL_GL_COLORSPACE,
	HorizontalResolution = EGL_HORIZONTAL_RESOLUTION,
	VerticalResolution   = EGL_VERTICAL_RESOLUTION,
	LargestPbuffer       = EGL_LARGEST_PBUFFER,
	MipmapTexture        = EGL_MIPMAP_TEXTURE,
	MipmapLevel          = EGL_MIPMAP_LEVEL,
	MultisampleResolve   = EGL_MULTISAMPLE_RESOLVE,
	PixelAspectRatio     = EGL_PIXEL_ASPECT_RATIO,
	RenderBuffer         = EGL_RENDER_BUFFER,
	SwapBehavior         = EGL_SWAP_BEHAVIOR,
	TextureFormat        = EGL_TEXTURE_FORMAT,
	TextureTarget        = EGL_TEXTURE_TARGET
};

template <>
__Range<SurfaceAttrib> __EnumValueRange<SurfaceAttrib>(void);

__StrCRef __EnumValueName(SurfaceAttrib);

//]
