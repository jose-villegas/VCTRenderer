//  File doc/quickbook/oglplus/quickref/enums/texture_wrap.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_wrap.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_wrap

enum class TextureWrap : GLenum
{
	ClampToEdge       = GL_CLAMP_TO_EDGE,
	Repeat            = GL_REPEAT,
	ClampToBorder     = GL_CLAMP_TO_BORDER,
	MirroredRepeat    = GL_MIRRORED_REPEAT,
	MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
};

template <>
__Range<TextureWrap> __EnumValueRange<TextureWrap>(void);

__StrCRef __EnumValueName(TextureWrap);

//]
