//  File doc/quickbook/oglplus/quickref/enums/primitive_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/primitive_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_primitive_type

enum class PrimitiveType : GLenum
{
	Points                 = GL_POINTS,
	LineStrip              = GL_LINE_STRIP,
	LineLoop               = GL_LINE_LOOP,
	Lines                  = GL_LINES,
	TriangleStrip          = GL_TRIANGLE_STRIP,
	TriangleFan            = GL_TRIANGLE_FAN,
	Triangles              = GL_TRIANGLES,
	LinesAdjacency         = GL_LINES_ADJACENCY,
	LineStripAdjacency     = GL_LINE_STRIP_ADJACENCY,
	TrianglesAdjacency     = GL_TRIANGLES_ADJACENCY,
	TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
	Patches                = GL_PATCHES
};

template <>
__Range<PrimitiveType> __EnumValueRange<PrimitiveType>(void);

__StrCRef __EnumValueName(PrimitiveType);

//]
