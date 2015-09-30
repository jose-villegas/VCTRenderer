//  File doc/quickbook/oglplus/quickref/enums/texture_swizzle_coord.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_swizzle_coord.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_swizzle_coord

enum class TextureSwizzleCoord : GLenum
{
	R    = GL_TEXTURE_SWIZZLE_R,
	G    = GL_TEXTURE_SWIZZLE_G,
	B    = GL_TEXTURE_SWIZZLE_B,
	A    = GL_TEXTURE_SWIZZLE_A,
	RGBA = GL_TEXTURE_SWIZZLE_RGBA
};

template <>
__Range<TextureSwizzleCoord> __EnumValueRange<TextureSwizzleCoord>(void);

__StrCRef __EnumValueName(TextureSwizzleCoord);

//]
