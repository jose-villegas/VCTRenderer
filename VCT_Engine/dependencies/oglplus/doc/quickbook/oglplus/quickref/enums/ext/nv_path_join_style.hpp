//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_join_style.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_join_style.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_join_style

enum class PathNVJoinStyle : GLenum
{
	None          = GL_NONE,
	Round         = GL_ROUND_NV,
	Bevel         = GL_BEVEL_NV,
	MiterRevert   = GL_MITER_REVERT_NV,
	MiterTruncate = GL_MITER_TRUNCATE_NV
};

template <>
__Range<PathNVJoinStyle> __EnumValueRange<PathNVJoinStyle>(void);

__StrCRef __EnumValueName(PathNVJoinStyle);

//]
