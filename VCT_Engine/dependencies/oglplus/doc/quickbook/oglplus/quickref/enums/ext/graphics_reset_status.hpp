//  File doc/quickbook/oglplus/quickref/enums/ext/graphics_reset_status.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/graphics_reset_status.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_graphics_reset_status

enum class GraphicsResetStatusARB : GLenum
{
	NoError              = GL_NO_ERROR,
	GuiltyContextReset   = GL_GUILTY_CONTEXT_RESET_ARB,
	InnocentContextReset = GL_INNOCENT_CONTEXT_RESET_ARB,
	UnknownContextReset  = GL_UNKNOWN_CONTEXT_RESET_ARB
};

template <>
__Range<GraphicsResetStatusARB> __EnumValueRange<GraphicsResetStatusARB>(void);

__StrCRef __EnumValueName(GraphicsResetStatusARB);

//]
