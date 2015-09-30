//  File doc/quickbook/eglplus/quickref/enums/string_query.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/string_query.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_string_query

enum class StringQuery : EGLenum
{
	ClientAPIs = EGL_CLIENT_APIS,
	Extensions = EGL_EXTENSIONS,
	Vendor     = EGL_VENDOR,
	Version    = EGL_VERSION
};

template <>
__Range<StringQuery> __EnumValueRange<StringQuery>(void);

__StrCRef __EnumValueName(StringQuery);

//]
