//  File doc/quickbook/oglplus/quickref/enums/buffer_usage.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/buffer_usage.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_buffer_usage

enum class BufferUsage : GLenum
{
	StreamDraw  = GL_STREAM_DRAW,
	StreamRead  = GL_STREAM_READ,
	StreamCopy  = GL_STREAM_COPY,
	StaticDraw  = GL_STATIC_DRAW,
	StaticRead  = GL_STATIC_READ,
	StaticCopy  = GL_STATIC_COPY,
	DynamicDraw = GL_DYNAMIC_DRAW,
	DynamicRead = GL_DYNAMIC_READ,
	DynamicCopy = GL_DYNAMIC_COPY
};

template <>
__Range<BufferUsage> __EnumValueRange<BufferUsage>(void);

__StrCRef __EnumValueName(BufferUsage);

//]
