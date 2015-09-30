//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_font_style.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_font_style.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_font_style

enum class PathNVFontStyle : GLbitfield
{
	Bold   = GL_BOLD_BIT_NV,
	Italic = GL_ITALIC_BIT_NV
};

template <>
__Range<PathNVFontStyle> __EnumValueRange<PathNVFontStyle>(void);

__StrCRef __EnumValueName(PathNVFontStyle);

__Bitfield<PathNVFontStyle> operator | (PathNVFontStyle b1, PathNVFontStyle b2);

//]
