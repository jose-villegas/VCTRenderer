//  File doc/quickbook/oglplus/quickref/enums/buffer_select_bit.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/buffer_select_bit.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_buffer_select_bit

enum class BufferSelectBit : GLbitfield
{
	ColorBuffer   = GL_COLOR_BUFFER_BIT,
	DepthBuffer   = GL_DEPTH_BUFFER_BIT,
	StencilBuffer = GL_STENCIL_BUFFER_BIT
};

template <>
__Range<BufferSelectBit> __EnumValueRange<BufferSelectBit>(void);

__StrCRef __EnumValueName(BufferSelectBit);

__Bitfield<BufferSelectBit> operator | (BufferSelectBit b1, BufferSelectBit b2);

//]
