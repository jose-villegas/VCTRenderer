//  File doc/quickbook/oglplus/quickref/enums/texture_wrap_coord.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_wrap_coord.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_wrap_coord

enum class TextureWrapCoord : GLenum
{
	S = GL_TEXTURE_WRAP_S,
	T = GL_TEXTURE_WRAP_T,
	R = GL_TEXTURE_WRAP_R
};

template <>
__Range<TextureWrapCoord> __EnumValueRange<TextureWrapCoord>(void);

__StrCRef __EnumValueName(TextureWrapCoord);

//]
