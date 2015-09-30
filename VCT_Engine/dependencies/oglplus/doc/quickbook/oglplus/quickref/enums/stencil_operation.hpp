//  File doc/quickbook/oglplus/quickref/enums/stencil_operation.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/stencil_operation.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_stencil_operation

enum class StencilOperation : GLenum
{
	Keep     = GL_KEEP,
	Zero     = GL_ZERO,
	Replace  = GL_REPLACE,
	Incr     = GL_INCR,
	Decr     = GL_DECR,
	Invert   = GL_INVERT,
	IncrWrap = GL_INCR_WRAP,
	DecrWrap = GL_DECR_WRAP
};

template <>
__Range<StencilOperation> __EnumValueRange<StencilOperation>(void);

__StrCRef __EnumValueName(StencilOperation);

//]
