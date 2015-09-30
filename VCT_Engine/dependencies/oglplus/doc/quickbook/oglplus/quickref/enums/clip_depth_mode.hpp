//  File doc/quickbook/oglplus/quickref/enums/clip_depth_mode.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/clip_depth_mode.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_clip_depth_mode

enum class ClipDepthMode : GLenum
{
	NegativeOneToOne = GL_NEGATIVE_ONE_TO_ONE,
	ZeroToOne        = GL_ZERO_TO_ONE
};

template <>
__Range<ClipDepthMode> __EnumValueRange<ClipDepthMode>(void);

__StrCRef __EnumValueName(ClipDepthMode);

//]
