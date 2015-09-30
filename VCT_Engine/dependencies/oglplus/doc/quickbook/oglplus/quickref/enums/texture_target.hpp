//  File doc/quickbook/oglplus/quickref/enums/texture_target.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/texture_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_texture_target

enum class TextureTarget : GLenum
{
	_1D                 = GL_TEXTURE_1D,
	_2D                 = GL_TEXTURE_2D,
	_3D                 = GL_TEXTURE_3D,
	_1DArray            = GL_TEXTURE_1D_ARRAY,
	_2DArray            = GL_TEXTURE_2D_ARRAY,
	Rectangle           = GL_TEXTURE_RECTANGLE,
	Buffer              = GL_TEXTURE_BUFFER,
	CubeMap             = GL_TEXTURE_CUBE_MAP,
	CubeMapArray        = GL_TEXTURE_CUBE_MAP_ARRAY,
	_2DMultisample      = GL_TEXTURE_2D_MULTISAMPLE,
	_2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
	CubeMapPositiveX    = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	CubeMapNegativeX    = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	CubeMapPositiveY    = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	CubeMapNegativeY    = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	CubeMapPositiveZ    = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	CubeMapNegativeZ    = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

template <>
__Range<TextureTarget> __EnumValueRange<TextureTarget>(void);

__StrCRef __EnumValueName(TextureTarget);

//]
