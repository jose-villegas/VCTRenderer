//  File doc/quickbook/oglplus/quickref/enums/ext/compat_matrix_mode.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/compat_matrix_mode.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_compat_matrix_mode

enum class CompatibilityMatrixMode : GLenum
{
	Projection = GL_PROJECTION,
	Modelview  = GL_MODELVIEW,
	Texture    = GL_TEXTURE,
	Color      = GL_COLOR
};

template <>
__Range<CompatibilityMatrixMode> __EnumValueRange<CompatibilityMatrixMode>(void);

__StrCRef __EnumValueName(CompatibilityMatrixMode);

//]
