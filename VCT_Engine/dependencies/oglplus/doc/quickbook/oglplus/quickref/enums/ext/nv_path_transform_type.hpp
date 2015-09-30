//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_transform_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_transform_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_transform_type

enum class PathNVTransformType : GLenum
{
	None              = GL_NONE,
	TranslateX        = GL_TRANSLATE_X_NV,
	TranslateY        = GL_TRANSLATE_Y_NV,
	Translate2D       = GL_TRANSLATE_2D_NV,
	Translate3D       = GL_TRANSLATE_3D_NV,
	Affine2D          = GL_AFFINE_2D_NV,
	Affine3D          = GL_AFFINE_3D_NV,
	TransposeAffine2D = GL_TRANSPOSE_AFFINE_2D_NV,
	TransposeAffine3D = GL_TRANSPOSE_AFFINE_3D_NV
};

template <>
__Range<PathNVTransformType> __EnumValueRange<PathNVTransformType>(void);

__StrCRef __EnumValueName(PathNVTransformType);

//]
