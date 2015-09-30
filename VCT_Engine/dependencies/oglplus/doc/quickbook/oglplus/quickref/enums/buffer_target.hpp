//  File doc/quickbook/oglplus/quickref/enums/buffer_target.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/buffer_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_buffer_target

enum class BufferTarget : GLenum
{
	Array                 = GL_ARRAY_BUFFER,
	AtomicCounter         = GL_ATOMIC_COUNTER_BUFFER,
	CopyRead              = GL_COPY_READ_BUFFER,
	CopyWrite             = GL_COPY_WRITE_BUFFER,
	DispatchIndirect      = GL_DISPATCH_INDIRECT_BUFFER,
	DrawIndirect          = GL_DRAW_INDIRECT_BUFFER,
	ElementArray          = GL_ELEMENT_ARRAY_BUFFER,
	PixelPack             = GL_PIXEL_PACK_BUFFER,
	PixelUnpack           = GL_PIXEL_UNPACK_BUFFER,
	ShaderStorage         = GL_SHADER_STORAGE_BUFFER,
	Texture               = GL_TEXTURE_BUFFER,
	TransformFeedback     = GL_TRANSFORM_FEEDBACK_BUFFER,
	Uniform               = GL_UNIFORM_BUFFER,
	Query                 = GL_QUERY_BUFFER,
	Parameter             = GL_PARAMETER_BUFFER_ARB,
	ExternalVirtualMemory = GL_EXTERNAL_VIRTUAL_MEMORY_BUFFER_AMD
};

template <>
__Range<BufferTarget> __EnumValueRange<BufferTarget>(void);

__StrCRef __EnumValueName(BufferTarget);

//]
