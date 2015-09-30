//  File doc/quickbook/oglplus/quickref/enums/ext/compat_prim_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/compat_prim_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_compat_prim_type

enum class CompatibilityPrimitiveType : GLenum
{
	Points        = GL_POINTS,
	LineStrip     = GL_LINE_STRIP,
	LineLoop      = GL_LINE_LOOP,
	Lines         = GL_LINES,
	TriangleStrip = GL_TRIANGLE_STRIP,
	TriangleFan   = GL_TRIANGLE_FAN,
	Triangles     = GL_TRIANGLES,
	Quads         = GL_QUADS,
	QuadStrip     = GL_QUAD_STRIP,
	Polygon       = GL_POLYGON
};

template <>
__Range<CompatibilityPrimitiveType> __EnumValueRange<CompatibilityPrimitiveType>(void);

__StrCRef __EnumValueName(CompatibilityPrimitiveType);

//]
