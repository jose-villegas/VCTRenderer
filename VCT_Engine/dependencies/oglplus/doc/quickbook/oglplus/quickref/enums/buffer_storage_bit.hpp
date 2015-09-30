//  File doc/quickbook/oglplus/quickref/enums/buffer_storage_bit.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/buffer_storage_bit.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_buffer_storage_bit

enum class BufferStorageBit : GLbitfield
{
	MapRead        = GL_MAP_READ_BIT,
	MapWrite       = GL_MAP_WRITE_BIT,
	MapPersistent  = GL_MAP_PERSISTENT_BIT,
	MapCoherent    = GL_MAP_COHERENT_BIT,
	DynamicStorage = GL_DYNAMIC_STORAGE_BIT,
	ClientStorage  = GL_CLIENT_STORAGE_BIT,
	SparseStorage  = GL_SPARSE_STORAGE_BIT_ARB
};

template <>
__Range<BufferStorageBit> __EnumValueRange<BufferStorageBit>(void);

__StrCRef __EnumValueName(BufferStorageBit);

__Bitfield<BufferStorageBit> operator | (BufferStorageBit b1, BufferStorageBit b2);

//]
