//  File doc/quickbook/oglplus/quickref/enums/program_interface.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/program_interface.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_program_interface

enum class ProgramInterface : GLenum
{
	Uniform                         = GL_UNIFORM,
	UniformBlock                    = GL_UNIFORM_BLOCK,
	AtomicCounterBuffer             = GL_ATOMIC_COUNTER_BUFFER,
	ProgramInput                    = GL_PROGRAM_INPUT,
	ProgramOutput                   = GL_PROGRAM_OUTPUT,
	VertexSubroutine                = GL_VERTEX_SUBROUTINE,
	TessControlSubroutine           = GL_TESS_CONTROL_SUBROUTINE,
	TessEvaluationSubroutine        = GL_TESS_EVALUATION_SUBROUTINE,
	GeometrySubroutine              = GL_GEOMETRY_SUBROUTINE,
	FragmentSubroutine              = GL_FRAGMENT_SUBROUTINE,
	ComputeSubroutine               = GL_COMPUTE_SUBROUTINE,
	VertexSubroutineUniform         = GL_VERTEX_SUBROUTINE_UNIFORM,
	TessControlSubroutineUniform    = GL_TESS_CONTROL_SUBROUTINE_UNIFORM,
	TessEvaluationSubroutineUniform = GL_TESS_EVALUATION_SUBROUTINE_UNIFORM,
	GeometrySubroutineUniform       = GL_GEOMETRY_SUBROUTINE_UNIFORM,
	FragmentSubroutineUniform       = GL_FRAGMENT_SUBROUTINE_UNIFORM,
	ComputeSubroutineUniform        = GL_COMPUTE_SUBROUTINE_UNIFORM,
	TransformFeedebackVarying       = GL_TRANSFORM_FEEDEBACK_VARYING,
	BufferVariable                  = GL_BUFFER_VARIABLE,
	ShaderStorageBlock              = GL_SHADER_STORAGE_BLOCK
};

template <>
__Range<ProgramInterface> __EnumValueRange<ProgramInterface>(void);

__StrCRef __EnumValueName(ProgramInterface);

//]
