//  File doc/quickbook/oglplus/quickref/enums/context_profile_bit.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/context_profile_bit.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_context_profile_bit

enum class ContextProfileBit : GLbitfield
{
	Core          = GL_CONTEXT_CORE_PROFILE_BIT,
	Compatibility = GL_CONTEXT_COMPATIBILITY_PROFILE_BIT
};

template <>
__Range<ContextProfileBit> __EnumValueRange<ContextProfileBit>(void);

__StrCRef __EnumValueName(ContextProfileBit);

__Bitfield<ContextProfileBit> operator | (ContextProfileBit b1, ContextProfileBit b2);

//]
