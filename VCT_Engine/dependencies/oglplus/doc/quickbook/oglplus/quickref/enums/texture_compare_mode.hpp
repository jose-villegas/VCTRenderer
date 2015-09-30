//  File doc/quickbook/oglplus/quickref/enums/texture_compare_mode.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_compare_mode.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_compare_mode

enum class TextureCompareMode : GLenum
{
	None                = GL_NONE,
	CompareRefToTexture = GL_COMPARE_REF_TO_TEXTURE
};

template <>
__Range<TextureCompareMode> __EnumValueRange<TextureCompareMode>(void);

__StrCRef __EnumValueName(TextureCompareMode);

//]
