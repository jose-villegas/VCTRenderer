//  File doc/quickbook/oglplus/quickref/enums/pixel_data_internal_format.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/pixel_data_internal_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_pixel_data_internal_format

enum class PixelDataInternalFormat : GLenum
{
	DepthComponent                        = GL_DEPTH_COMPONENT,
	DepthStencil                          = GL_DEPTH_STENCIL,
	StencilIndex8                         = GL_STENCIL_INDEX8,
	Red                                   = GL_RED,
	RG                                    = GL_RG,
	RGB                                   = GL_RGB,
	RGBA                                  = GL_RGBA,
	R8                                    = GL_R8,
	R8SNorm                               = GL_R8_SNORM,
	R16                                   = GL_R16,
	R16SNorm                              = GL_R16_SNORM,
	RG8                                   = GL_RG8,
	RG8SNorm                              = GL_RG8_SNORM,
	RG16                                  = GL_RG16,
	RG16SNorm                             = GL_RG16_SNORM,
	R3G3B2                                = GL_R3_G3_B2,
	RGB4                                  = GL_RGB4,
	RGB5                                  = GL_RGB5,
	RGB8                                  = GL_RGB8,
	RGB8SNorm                             = GL_RGB8_SNORM,
	RGB10                                 = GL_RGB10,
	RGB12                                 = GL_RGB12,
	RGB16                                 = GL_RGB16,
	RGB16SNorm                            = GL_RGB16_SNORM,
	RGBA2                                 = GL_RGBA2,
	RGBA4                                 = GL_RGBA4,
	RGB5A1                                = GL_RGB5_A1,
	RGBA8                                 = GL_RGBA8,
	RGBA8SNorm                            = GL_RGBA8_SNORM,
	RGB10A2                               = GL_RGB10_A2,
	RGB10A2UI                             = GL_RGB10_A2UI,
	RGBA12                                = GL_RGBA12,
	RGBA16                                = GL_RGBA16,
	RGBA16SNorm                           = GL_RGBA16_SNORM,
	SRGB8                                 = GL_SRGB8,
	SRGB8Alpha8                           = GL_SRGB8_ALPHA8,
	R16F                                  = GL_R16F,
	RG16F                                 = GL_RG16F,
	RGB16F                                = GL_RGB16F,
	RGBA16F                               = GL_RGBA16F,
	R32F                                  = GL_R32F,
	RG32F                                 = GL_RG32F,
	RGB32F                                = GL_RGB32F,
	RGBA32F                               = GL_RGBA32F,
	R11FG11FB10F                          = GL_R11F_G11F_B10F,
	RGB9E5                                = GL_RGB9_E5,
	R8I                                   = GL_R8I,
	R8UI                                  = GL_R8UI,
	R16I                                  = GL_R16I,
	R16UI                                 = GL_R16UI,
	R32I                                  = GL_R32I,
	R32UI                                 = GL_R32UI,
	RG8I                                  = GL_RG8I,
	RG8UI                                 = GL_RG8UI,
	RG16I                                 = GL_RG16I,
	RG16UI                                = GL_RG16UI,
	RG32I                                 = GL_RG32I,
	RG32UI                                = GL_RG32UI,
	RGB8I                                 = GL_RGB8I,
	RGB8UI                                = GL_RGB8UI,
	RGB16I                                = GL_RGB16I,
	RGB16UI                               = GL_RGB16UI,
	RGB32I                                = GL_RGB32I,
	RGB32UI                               = GL_RGB32UI,
	RGBA8I                                = GL_RGBA8I,
	RGBA8UI                               = GL_RGBA8UI,
	RGBA16I                               = GL_RGBA16I,
	RGBA16UI                              = GL_RGBA16UI,
	RGBA32I                               = GL_RGBA32I,
	RGBA32UI                              = GL_RGBA32UI,
	DepthComponent16                      = GL_DEPTH_COMPONENT16,
	DepthComponent24                      = GL_DEPTH_COMPONENT24,
	DepthComponent32                      = GL_DEPTH_COMPONENT32,
	DepthComponent32F                     = GL_DEPTH_COMPONENT32F,
	Depth24Stencil8                       = GL_DEPTH24_STENCIL8,
	Depth32fStencil8                      = GL_DEPTH32F_STENCIL8,
	CompressedRed                         = GL_COMPRESSED_RED,
	CompressedRG                          = GL_COMPRESSED_RG,
	CompressedRGB                         = GL_COMPRESSED_RGB,
	CompressedRGBA                        = GL_COMPRESSED_RGBA,
	CompressedSRGB                        = GL_COMPRESSED_SRGB,
	CompressedSRGBAlpha                   = GL_COMPRESSED_SRGB_ALPHA,
	CompressedRedRGTC1                    = GL_COMPRESSED_RED_RGTC1,
	CompressedSignedRedRGTC1              = GL_COMPRESSED_SIGNED_RED_RGTC1,
	CompressedRGRGTC2                     = GL_COMPRESSED_RG_RGTC2,
	CompressedSignedRGRGTC2               = GL_COMPRESSED_SIGNED_RG_RGTC2,
	CompressedRGBABPTCUNorm               = GL_COMPRESSED_RGBA_BPTC_UNORM,
	CompressedSRGBAlphaBPTCUNorm          = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
	CompressedRGBBPTCSignedFloat          = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
	CompressedRGBBPTCUnsignedFloat        = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,
	CompressedRGB8ETC2                    = GL_COMPRESSED_RGB8_ETC2,
	CompressedSRGB8ETC2                   = GL_COMPRESSED_SRGB8_ETC2,
	CompressedRGB8PunchthroughAlpha1ETC2  = GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,
	CompressedSRGB8PunchthroughAlpha1ETC2 = GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2,
	CompressedETC2EAC                     = GL_COMPRESSED_RGBA8_ETC2_EAC,
	CompressedSRGB8Alpha8ETC2EAC          = GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC,
	CompressedR11EAC                      = GL_COMPRESSED_R11_EAC,
	CompressedSignedR11EAC                = GL_COMPRESSED_SIGNED_R11_EAC,
	CompressedRG11EAC                     = GL_COMPRESSED_RG11_EAC,
	CompressedSignedRG11EAC               = GL_COMPRESSED_SIGNED_RG11_EAC
};

template <>
__Range<PixelDataInternalFormat> __EnumValueRange<PixelDataInternalFormat>(void);

__StrCRef __EnumValueName(PixelDataInternalFormat);

//]
