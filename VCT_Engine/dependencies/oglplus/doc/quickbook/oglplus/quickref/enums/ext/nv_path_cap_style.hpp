//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_cap_style.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_cap_style.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_cap_style

enum class PathNVCapStyle : GLenum
{
	Flat       = GL_FLAT,
	Square     = GL_SQUARE_NV,
	Round      = GL_ROUND_NV,
	Triangular = GL_TRIANGULAR_NV
};

template <>
__Range<PathNVCapStyle> __EnumValueRange<PathNVCapStyle>(void);

__StrCRef __EnumValueName(PathNVCapStyle);

//]
