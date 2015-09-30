//  File doc/quickbook/oglplus/quickref/enums/hint_option.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/hint_option.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_hint_option

enum class HintOption : GLenum
{
	Fastest  = GL_FASTEST,
	Nicest   = GL_NICEST,
	DontCare = GL_DONT_CARE
};

template <>
__Range<HintOption> __EnumValueRange<HintOption>(void);

__StrCRef __EnumValueName(HintOption);

//]
