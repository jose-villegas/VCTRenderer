//  File doc/quickbook/eglplus/quickref/enums/texture_target.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/texture_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_texture_target

enum class TextureTarget : EGLenum
{
	NoTexture = EGL_NO_TEXTURE,
	Texture2D = EGL_TEXTURE_2D
};

template <>
__Range<TextureTarget> __EnumValueRange<TextureTarget>(void);

__StrCRef __EnumValueName(TextureTarget);

//]
