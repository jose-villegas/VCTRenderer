//  File doc/quickbook/oglplus/quickref/enums/provoke_mode.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/provoke_mode.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_provoke_mode

enum class ProvokeMode : GLenum
{
	FirstVertexConvention = GL_FIRST_VERTEX_CONVENTION,
	LastVertexConvention  = GL_LAST_VERTEX_CONVENTION
};

template <>
__Range<ProvokeMode> __EnumValueRange<ProvokeMode>(void);

__StrCRef __EnumValueName(ProvokeMode);

//]
