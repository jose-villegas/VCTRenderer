//  File doc/quickbook/oglplus/quickref/enums/context_flag_bit.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/context_flag_bit.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_context_flag_bit

enum class ContextFlagBit : GLbitfield
{
	ForwardCompatible = GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT,
	Debug             = GL_CONTEXT_FLAG_DEBUG_BIT,
	RobustAccess      = GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB
};

template <>
__Range<ContextFlagBit> __EnumValueRange<ContextFlagBit>(void);

__StrCRef __EnumValueName(ContextFlagBit);

__Bitfield<ContextFlagBit> operator | (ContextFlagBit b1, ContextFlagBit b2);

//]
