//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_format.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_format

enum class PathNVFormat : GLenum
{
	SVG = GL_PATH_FORMAT_SVG_NV,
	PS  = GL_PATH_FORMAT_PS_NV
};

template <>
__Range<PathNVFormat> __EnumValueRange<PathNVFormat>(void);

__StrCRef __EnumValueName(PathNVFormat);

//]
