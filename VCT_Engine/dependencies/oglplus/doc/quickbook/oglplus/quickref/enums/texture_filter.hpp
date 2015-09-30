//  File doc/quickbook/oglplus/quickref/enums/texture_filter.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_filter.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_filter

enum class TextureFilter : GLenum
{
	Nearest = GL_NEAREST,
	Linear  = GL_LINEAR
};

template <>
__Range<TextureFilter> __EnumValueRange<TextureFilter>(void);

__StrCRef __EnumValueName(TextureFilter);

//]
