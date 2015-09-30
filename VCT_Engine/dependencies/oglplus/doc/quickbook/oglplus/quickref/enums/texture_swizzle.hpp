//  File doc/quickbook/oglplus/quickref/enums/texture_swizzle.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_swizzle.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_swizzle

enum class TextureSwizzle : GLenum
{
	Red   = GL_RED,
	Green = GL_GREEN,
	Blue  = GL_BLUE,
	Alpha = GL_ALPHA,
	Zero  = GL_ZERO,
	One   = GL_ONE
};

template <>
__Range<TextureSwizzle> __EnumValueRange<TextureSwizzle>(void);

__StrCRef __EnumValueName(TextureSwizzle);

//]
