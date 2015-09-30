//  File doc/quickbook/oglplus/quickref/enums/query_target.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/query_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_query_target

enum class QueryTarget : GLenum
{
	TimeElapsed                        = GL_TIME_ELAPSED,
	Timestamp                          = GL_TIMESTAMP,
	SamplesPassed                      = GL_SAMPLES_PASSED,
	AnySamplesPassed                   = GL_ANY_SAMPLES_PASSED,
	PrimitivesGenerated                = GL_PRIMITIVES_GENERATED,
	TransformFeedbackPrimitivesWritten = GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,
	VerticesSubmitted                  = GL_VERTICES_SUBMITTED_ARB,
	PrimitivesSubmitted                = GL_PRIMITIVES_SUBMITTED_ARB,
	VertexShaderInvocations            = GL_VERTEX_SHADER_INVOCATIONS_ARB,
	TessControlShaderPatches           = GL_TESS_CONTROL_SHADER_PATCHES_ARB,
	TessEvaluationShaderInvocations    = GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB,
	GeometryShaderInvocations          = GL_GEOMETRY_SHADER_INVOCATIONS,
	GeometryShaderPrimitivesEmitted    = GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB,
	FragmentShaderInvocations          = GL_FRAGMENT_SHADER_INVOCATIONS_ARB,
	ComputeShaderInvocations           = GL_COMPUTE_SHADER_INVOCATIONS_ARB,
	ClippingInputPrimitives            = GL_CLIPPING_INPUT_PRIMITIVES_ARB,
	ClippingOutputPrimitives           = GL_CLIPPING_OUTPUT_PRIMITIVES_ARB,
	TransformFeedbackOverflow          = GL_TRANSFORM_FEEDBACK_OVERFLOW_ARB,
	TransformFeedbackStreamOverflow    = GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW_ARB
};

template <>
__Range<QueryTarget> __EnumValueRange<QueryTarget>(void);

__StrCRef __EnumValueName(QueryTarget);

//]
