//  File doc/quickbook/oglplus/quickref/enums/clip_origin.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/clip_origin.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_clip_origin

enum class ClipOrigin : GLenum
{
	LowerLeft = GL_LOWER_LEFT,
	UpperLeft = GL_UPPER_LEFT
};

template <>
__Range<ClipOrigin> __EnumValueRange<ClipOrigin>(void);

__StrCRef __EnumValueName(ClipOrigin);

//]
