//  File doc/quickbook/oglplus/quickref/enums/compare_function.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/compare_function.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_compare_function

enum class CompareFunction : GLenum
{
	LEqual   = GL_LEQUAL,
	GEqual   = GL_GEQUAL,
	Less     = GL_LESS,
	Greater  = GL_GREATER,
	Equal    = GL_EQUAL,
	NotEqual = GL_NOTEQUAL,
	Always   = GL_ALWAYS,
	Never    = GL_NEVER
};

template <>
__Range<CompareFunction> __EnumValueRange<CompareFunction>(void);

__StrCRef __EnumValueName(CompareFunction);

//]
