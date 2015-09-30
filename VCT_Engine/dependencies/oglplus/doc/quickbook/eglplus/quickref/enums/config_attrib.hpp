//  File doc/quickbook/eglplus/quickref/enums/config_attrib.hpp
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
//[eglplus_enums_config_attrib

enum class ConfigAttrib : EGLenum
{
	BufferSize            = EGL_BUFFER_SIZE,
	RedSize               = EGL_RED_SIZE,
	GreenSize             = EGL_GREEN_SIZE,
	BlueSize              = EGL_BLUE_SIZE,
	LuminanceSize         = EGL_LUMINANCE_SIZE,
	AlphaSize             = EGL_ALPHA_SIZE,
	AlphaMaskSize         = EGL_ALPHA_MASK_SIZE,
	BindToTextureRGB      = EGL_BIND_TO_TEXTURE_RGB,
	BindToTextureRGBA     = EGL_BIND_TO_TEXTURE_RGBA,
	ColorBufferType       = EGL_COLOR_BUFFER_TYPE,
	ConfigCaveat          = EGL_CONFIG_CAVEAT,
	ConfigId              = EGL_CONFIG_ID,
	Conformant            = EGL_CONFORMANT,
	DepthSize             = EGL_DEPTH_SIZE,
	Level                 = EGL_LEVEL,
	MaxPbufferWidth       = EGL_MAX_PBUFFER_WIDTH,
	MaxPbufferHeight      = EGL_MAX_PBUFFER_HEIGHT,
	MaxPbufferPixels      = EGL_MAX_PBUFFER_PIXELS,
	MaxSwapInterval       = EGL_MAX_SWAP_INTERVAL,
	MinSwapInterval       = EGL_MIN_SWAP_INTERVAL,
	NativeRenderable      = EGL_NATIVE_RENDERABLE,
	NativeVisualId        = EGL_NATIVE_VISUAL_ID,
	NativeVisualType      = EGL_NATIVE_VISUAL_TYPE,
	RenderableType        = EGL_RENDERABLE_TYPE,
	SampleBuffers         = EGL_SAMPLE_BUFFERS,
	Samples               = EGL_SAMPLES,
	StencilSize           = EGL_STENCIL_SIZE,
	SurfaceType           = EGL_SURFACE_TYPE,
	TransparentType       = EGL_TRANSPARENT_TYPE,
	TransparentRedValue   = EGL_TRANSPARENT_RED_VALUE,
	TransparentGreenValue = EGL_TRANSPARENT_GREEN_VALUE,
	TransparentBlueValue  = EGL_TRANSPARENT_BLUE_VALUE
};

template <>
__Range<ConfigAttrib> __EnumValueRange<ConfigAttrib>(void);

__StrCRef __EnumValueName(ConfigAttrib);

//]
