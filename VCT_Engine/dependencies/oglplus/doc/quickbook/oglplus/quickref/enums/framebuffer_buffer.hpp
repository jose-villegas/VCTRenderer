//  File doc/quickbook/oglplus/quickref/enums/framebuffer_buffer.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/framebuffer_buffer.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_framebuffer_buffer

enum class FramebufferBuffer : GLenum
{
	Color        = GL_COLOR,
	Depth        = GL_DEPTH,
	Stencil      = GL_STENCIL,
	DepthStencil = GL_DEPTH_STENCIL
};

template <>
__Range<FramebufferBuffer> __EnumValueRange<FramebufferBuffer>(void);

__StrCRef __EnumValueName(FramebufferBuffer);

//]
