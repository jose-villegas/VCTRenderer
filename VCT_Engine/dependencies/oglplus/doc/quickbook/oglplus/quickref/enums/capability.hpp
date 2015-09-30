//  File doc/quickbook/oglplus/quickref/enums/capability.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/capability.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_capability

enum class Capability : GLenum
{
	PrimitiveRestart        = GL_PRIMITIVE_RESTART,
	DepthTest               = GL_DEPTH_TEST,
	StencilTest             = GL_STENCIL_TEST,
	ScissorTest             = GL_SCISSOR_TEST,
	CullFace                = GL_CULL_FACE,
	RasterizerDiscard       = GL_RASTERIZER_DISCARD,
	PolygonOffsetPoint      = GL_POLYGON_OFFSET_POINT,
	PolygonOffsetLine       = GL_POLYGON_OFFSET_LINE,
	PolygonOffsetFill       = GL_POLYGON_OFFSET_FILL,
	Blend                   = GL_BLEND,
	ColorLogicOp            = GL_COLOR_LOGIC_OP,
	Dither                  = GL_DITHER,
	Multisample             = GL_MULTISAMPLE,
	SampleShading           = GL_SAMPLE_SHADING,
	LineSmooth              = GL_LINE_SMOOTH,
	PolygonSmooth           = GL_POLYGON_SMOOTH,
	ProgramPointSize        = GL_PROGRAM_POINT_SIZE,
	TextureCubeMapSeamless  = GL_TEXTURE_CUBE_MAP_SEAMLESS,
	SampleAlphaToCoverage   = GL_SAMPLE_ALPHA_TO_COVERAGE,
	SampleAlphaToOne        = GL_SAMPLE_ALPHA_TO_ONE,
	SampleCoverage          = GL_SAMPLE_COVERAGE,
	SampleMask              = GL_SAMPLE_MASK,
	FramebufferSRGB         = GL_FRAMEBUFFER_SRGB,
	DebugOutputSynchronous  = GL_DEBUG_OUTPUT_SYNCHRONOUS,
	StreamRasterization     = GL_STREAM_RASTERIZATION_AMD,
	BlendAdvancedCoherent   = GL_BLEND_ADVANCED_COHERENT_KHR,
	FragmentCoverageToColor = GL_FRAGMENT_COVERAGE_TO_COLOR_NV
};

template <>
__Range<Capability> __EnumValueRange<Capability>(void);

__StrCRef __EnumValueName(Capability);

//]
