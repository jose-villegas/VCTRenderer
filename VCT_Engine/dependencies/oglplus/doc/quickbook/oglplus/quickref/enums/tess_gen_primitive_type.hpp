//  File doc/quickbook/oglplus/quickref/enums/tess_gen_primitive_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/tess_gen_primitive_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_tess_gen_primitive_type

enum class TessGenPrimitiveType : GLenum
{
	Quads     = GL_QUADS,
	Triangles = GL_TRIANGLES,
	Isolines  = GL_ISOLINES
};

template <>
__Range<TessGenPrimitiveType> __EnumValueRange<TessGenPrimitiveType>(void);

__StrCRef __EnumValueName(TessGenPrimitiveType);

//]
