//  File doc/quickbook/oglplus/quickref/enums/framebuffer_attachment.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/framebuffer_attachment.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_framebuffer_attachment

enum class FramebufferAttachment : GLenum
{
	Color        = GL_COLOR_ATTACHMENT0,
	Color1       = GL_COLOR_ATTACHMENT1,
	Color2       = GL_COLOR_ATTACHMENT2,
	Color3       = GL_COLOR_ATTACHMENT3,
	Color4       = GL_COLOR_ATTACHMENT4,
	Color5       = GL_COLOR_ATTACHMENT5,
	Color6       = GL_COLOR_ATTACHMENT6,
	Color7       = GL_COLOR_ATTACHMENT7,
	Color8       = GL_COLOR_ATTACHMENT8,
	Color9       = GL_COLOR_ATTACHMENT9,
	Color10      = GL_COLOR_ATTACHMENT10,
	Color11      = GL_COLOR_ATTACHMENT11,
	Color12      = GL_COLOR_ATTACHMENT12,
	Color13      = GL_COLOR_ATTACHMENT13,
	Color14      = GL_COLOR_ATTACHMENT14,
	Color15      = GL_COLOR_ATTACHMENT15,
	Depth        = GL_DEPTH_ATTACHMENT,
	Stencil      = GL_STENCIL_ATTACHMENT,
	DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
};

template <>
__Range<FramebufferAttachment> __EnumValueRange<FramebufferAttachment>(void);

__StrCRef __EnumValueName(FramebufferAttachment);

//]
