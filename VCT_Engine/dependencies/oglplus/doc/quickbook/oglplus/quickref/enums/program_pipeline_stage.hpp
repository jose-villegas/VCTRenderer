//  File doc/quickbook/oglplus/quickref/enums/program_pipeline_stage.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/program_pipeline_stage.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_program_pipeline_stage

enum class ProgramPipelineStage : GLbitfield
{
	VertexShader         = GL_VERTEX_SHADER_BIT,
	TessControlShader    = GL_TESS_CONTROL_SHADER_BIT,
	TessEvaluationShader = GL_TESS_EVALUATION_SHADER_BIT,
	GeometryShader       = GL_GEOMETRY_SHADER_BIT,
	FragmentShader       = GL_FRAGMENT_SHADER_BIT,
	ComputeShader        = GL_COMPUTE_SHADER_BIT,
	AllShaders           = GL_ALL_SHADER_BITS
};

template <>
__Range<ProgramPipelineStage> __EnumValueRange<ProgramPipelineStage>(void);

__StrCRef __EnumValueName(ProgramPipelineStage);

__Bitfield<ProgramPipelineStage> operator | (ProgramPipelineStage b1, ProgramPipelineStage b2);

//]
