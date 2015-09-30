//  File doc/quickbook/oglplus/quickref/enums/conditional_render_mode.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/conditional_render_mode.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_conditional_render_mode

enum class ConditionalRenderMode : GLenum
{
	QueryWait                   = GL_QUERY_WAIT,
	QueryNoWait                 = GL_QUERY_NO_WAIT,
	QueryByRegionWait           = GL_QUERY_BY_REGION_WAIT,
	QueryByRegionNoWait         = GL_QUERY_BY_REGION_NO_WAIT,
	QueryWaitInverted           = GL_QUERY_WAIT_INVERTED,
	QueryNoWaitInverted         = GL_QUERY_NO_WAIT_INVERTED,
	QueryByRegionWaitInverted   = GL_QUERY_BY_REGION_WAIT_INVERTED,
	QueryByRegionNoWaitInverted = GL_QUERY_BY_REGION_NO_WAIT_INVERTED
};

template <>
__Range<ConditionalRenderMode> __EnumValueRange<ConditionalRenderMode>(void);

__StrCRef __EnumValueName(ConditionalRenderMode);

//]
