//  File doc/quickbook/oglplus/quickref/enums/hint_target.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/hint_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_hint_target

enum class HintTarget : GLenum
{
	LineSmooth               = GL_LINE_SMOOTH_HINT,
	PolygonSmooth            = GL_POLYGON_SMOOTH_HINT,
	TextureCompression       = GL_TEXTURE_COMPRESSION_HINT,
	FragmentShaderDerivative = GL_FRAGMENT_SHADER_DERIVATIVE_HINT
};

template <>
__Range<HintTarget> __EnumValueRange<HintTarget>(void);

__StrCRef __EnumValueName(HintTarget);

//]
