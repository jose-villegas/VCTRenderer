//  File doc/quickbook/oglplus/quickref/enums/pixel_parameter.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/pixel_parameter.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_pixel_parameter

enum class PixelParameter : GLenum
{
	PackSwapBytes               = GL_PACK_SWAP_BYTES,
	PackLSBFirst                = GL_PACK_LSB_FIRST,
	PackRowLength               = GL_PACK_ROW_LENGTH,
	PackSkipRows                = GL_PACK_SKIP_ROWS,
	PackSkipPixels              = GL_PACK_SKIP_PIXELS,
	PackAlignment               = GL_PACK_ALIGNMENT,
	PackImageHeight             = GL_PACK_IMAGE_HEIGHT,
	PackSkipImages              = GL_PACK_SKIP_IMAGES,
	PackCompressedBlockWidth    = GL_PACK_COMPRESSED_BLOCK_WIDTH,
	PackCompressedBlockHeight   = GL_PACK_COMPRESSED_BLOCK_HEIGHT,
	PackCompressedBlockDepth    = GL_PACK_COMPRESSED_BLOCK_DEPTH,
	PackCompressedBlockSize     = GL_PACK_COMPRESSED_BLOCK_SIZE,
	UnpackSwapBytes             = GL_UNPACK_SWAP_BYTES,
	UnpackLSBFirst              = GL_UNPACK_LSB_FIRST,
	UnpackRowLength             = GL_UNPACK_ROW_LENGTH,
	UnpackSkipRows              = GL_UNPACK_SKIP_ROWS,
	UnpackSkipPixels            = GL_UNPACK_SKIP_PIXELS,
	UnpackAlignment             = GL_UNPACK_ALIGNMENT,
	UnpackImageHeight           = GL_UNPACK_IMAGE_HEIGHT,
	UnpackSkipImages            = GL_UNPACK_SKIP_IMAGES,
	UnpackCompressedBlockWidth  = GL_UNPACK_COMPRESSED_BLOCK_WIDTH,
	UnpackCompressedBlockHeight = GL_UNPACK_COMPRESSED_BLOCK_HEIGHT,
	UnpackCompressedBlockDepth  = GL_UNPACK_COMPRESSED_BLOCK_DEPTH,
	UnpackCompressedBlockSize   = GL_UNPACK_COMPRESSED_BLOCK_SIZE
};

template <>
__Range<PixelParameter> __EnumValueRange<PixelParameter>(void);

__StrCRef __EnumValueName(PixelParameter);

//]
