//  File doc/quickbook/oglplus/quickref/enums/shader_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/shader_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_shader_type

enum class ShaderType : GLenum
{
	Vertex         = GL_VERTEX_SHADER,
	TessControl    = GL_TESS_CONTROL_SHADER,
	TessEvaluation = GL_TESS_EVALUATION_SHADER,
	Geometry       = GL_GEOMETRY_SHADER,
	Fragment       = GL_FRAGMENT_SHADER,
	Compute        = GL_COMPUTE_SHADER
};

template <>
__Range<ShaderType> __EnumValueRange<ShaderType>(void);

__StrCRef __EnumValueName(ShaderType);

//]
