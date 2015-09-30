//  File doc/quickbook/oglplus/quickref/enums/buffer_map_access.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/buffer_map_access.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_buffer_map_access

enum class BufferMapAccess : GLbitfield
{
	Read             = GL_MAP_READ_BIT,
	Write            = GL_MAP_WRITE_BIT,
	Persistent       = GL_MAP_PERSISTENT_BIT,
	Coherent         = GL_MAP_COHERENT_BIT,
	InvalidateRange  = GL_MAP_INVALIDATE_RANGE_BIT,
	InvalidateBuffer = GL_MAP_INVALIDATE_BUFFER_BIT,
	FlushExplicit    = GL_MAP_FLUSH_EXPLICIT_BIT,
	Unsynchronized   = GL_MAP_UNSYNCHRONIZED_BIT
};

template <>
__Range<BufferMapAccess> __EnumValueRange<BufferMapAccess>(void);

__StrCRef __EnumValueName(BufferMapAccess);

__Bitfield<BufferMapAccess> operator | (BufferMapAccess b1, BufferMapAccess b2);

//]
