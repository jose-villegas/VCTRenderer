//  File doc/quickbook/oglplus/quickref/enums/string_query.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/string_query.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_string_query

enum class StringQuery : GLenum
{
	Renderer               = GL_RENDERER,
	Vendor                 = GL_VENDOR,
	Version                = GL_VERSION,
	ShadingLanguageVersion = GL_SHADING_LANGUAGE_VERSION
};

template <>
__Range<StringQuery> __EnumValueRange<StringQuery>(void);

__StrCRef __EnumValueName(StringQuery);

//]
