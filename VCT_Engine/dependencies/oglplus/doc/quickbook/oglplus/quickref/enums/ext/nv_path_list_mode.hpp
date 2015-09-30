//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_list_mode.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_list_mode.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_list_mode

enum class PathNVListMode : GLenum
{
	AccumAdjacentPairs = GL_ACCUM_ADJACENT_PAIRS_NV,
	AdjacentPairs      = GL_ADJACENT_PAIRS_NV,
	FirstToRest        = GL_FIRST_TO_REST_NV
};

template <>
__Range<PathNVListMode> __EnumValueRange<PathNVListMode>(void);

__StrCRef __EnumValueName(PathNVListMode);

//]
