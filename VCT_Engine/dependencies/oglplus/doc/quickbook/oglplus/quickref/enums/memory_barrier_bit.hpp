//  File doc/quickbook/oglplus/quickref/enums/memory_barrier_bit.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/memory_barrier_bit.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_memory_barrier_bit

enum class MemoryBarrierBit : GLbitfield
{
	VertexAttribArray  = GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT,
	ElementArray       = GL_ELEMENT_ARRAY_BARRIER_BIT,
	Uniform            = GL_UNIFORM_BARRIER_BIT,
	TextureFetch       = GL_TEXTURE_FETCH_BARRIER_BIT,
	ShaderImageAccess  = GL_SHADER_IMAGE_ACCESS_BARRIER_BIT,
	Command            = GL_COMMAND_BARRIER_BIT,
	PixelBuffer        = GL_PIXEL_BUFFER_BARRIER_BIT,
	TextureUpdate      = GL_TEXTURE_UPDATE_BARRIER_BIT,
	BufferUpdate       = GL_BUFFER_UPDATE_BARRIER_BIT,
	Framebuffer        = GL_FRAMEBUFFER_BARRIER_BIT,
	TransformFeedback  = GL_TRANSFORM_FEEDBACK_BARRIER_BIT,
	AtomicCounter      = GL_ATOMIC_COUNTER_BARRIER_BIT,
	ShaderStorage      = GL_SHADER_STORAGE_BARRIER_BIT,
	ClientMappedBuffer = GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT,
	All                = GL_ALL_BARRIER_BITS
};

template <>
__Range<MemoryBarrierBit> __EnumValueRange<MemoryBarrierBit>(void);

__StrCRef __EnumValueName(MemoryBarrierBit);

__Bitfield<MemoryBarrierBit> operator | (MemoryBarrierBit b1, MemoryBarrierBit b2);

//]
