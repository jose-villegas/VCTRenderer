//  File doc/quickbook/oglplus/quickref/enums/context_release_behavior.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/context_release_behavior.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_context_release_behavior

enum class ContextReleaseBehavior : GLenum
{
	None  = GL_NONE,
	Flush = GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH
};

template <>
__Range<ContextReleaseBehavior> __EnumValueRange<ContextReleaseBehavior>(void);

__StrCRef __EnumValueName(ContextReleaseBehavior);

//]
