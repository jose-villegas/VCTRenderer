//  File doc/quickbook/eglplus/quickref/enums/swap_behavior.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/swap_behavior.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_swap_behavior

enum class SwapBehavior : EGLenum
{
	BufferPreserved = EGL_BUFFER_PRESERVED,
	BufferDestroyed = EGL_BUFFER_DESTROYED
};

template <>
__Range<SwapBehavior> __EnumValueRange<SwapBehavior>(void);

__StrCRef __EnumValueName(SwapBehavior);

//]
