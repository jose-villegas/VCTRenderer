//  File doc/quickbook/oglplus/quickref/enums/pixel_data_format.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/pixel_data_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_pixel_data_format

enum class PixelDataFormat : GLenum
{
	DepthComponent = GL_DEPTH_COMPONENT,
	DepthStencil   = GL_DEPTH_STENCIL,
	StencilIndex   = GL_STENCIL_INDEX,
	Red            = GL_RED,
	Green          = GL_GREEN,
	Blue           = GL_BLUE,
	RG             = GL_RG,
	RGB            = GL_RGB,
	RGBA           = GL_RGBA,
	BGR            = GL_BGR,
	BGRA           = GL_BGRA,
	RedInteger     = GL_RED_INTEGER,
	GreenInteger   = GL_GREEN_INTEGER,
	BlueInteger    = GL_BLUE_INTEGER,
	RGInteger      = GL_RG_INTEGER,
	RGBInteger     = GL_RGB_INTEGER,
	RGBAInteger    = GL_RGBA_INTEGER,
	BGRInteger     = GL_BGR_INTEGER,
	BGRAInteger    = GL_BGRA_INTEGER
};

template <>
__Range<PixelDataFormat> __EnumValueRange<PixelDataFormat>(void);

__StrCRef __EnumValueName(PixelDataFormat);

//]
