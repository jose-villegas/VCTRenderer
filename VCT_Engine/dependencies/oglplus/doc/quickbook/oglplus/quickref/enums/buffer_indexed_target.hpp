//  File doc/quickbook/oglplus/quickref/enums/buffer_indexed_target.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/buffer_indexed_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_buffer_indexed_target

enum class BufferIndexedTarget : GLenum
{
	AtomicCounter     = GL_ATOMIC_COUNTER_BUFFER,
	ShaderStorage     = GL_SHADER_STORAGE_BUFFER,
	TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
	Uniform           = GL_UNIFORM_BUFFER
};

template <>
__Range<BufferIndexedTarget> __EnumValueRange<BufferIndexedTarget>(void);

__StrCRef __EnumValueName(BufferIndexedTarget);

//]
