//  File doc/quickbook/oglplus/quickref/enums/capability_class.hpp
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
//[oglplus_enums_capability_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__Capability> class Transform>
class __EnumToClass<Base, __Capability, Transform> /*<
Specialization of __EnumToClass for the __Capability enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<Capability::PrimitiveRestart>
		PrimitiveRestart;
	Transform<Capability::DepthTest>
		DepthTest;
	Transform<Capability::StencilTest>
		StencilTest;
	Transform<Capability::ScissorTest>
		ScissorTest;
	Transform<Capability::CullFace>
		CullFace;
	Transform<Capability::RasterizerDiscard>
		RasterizerDiscard;
	Transform<Capability::PolygonOffsetPoint>
		PolygonOffsetPoint;
	Transform<Capability::PolygonOffsetLine>
		PolygonOffsetLine;
	Transform<Capability::PolygonOffsetFill>
		PolygonOffsetFill;
	Transform<Capability::Blend>
		Blend;
	Transform<Capability::ColorLogicOp>
		ColorLogicOp;
	Transform<Capability::Dither>
		Dither;
	Transform<Capability::Multisample>
		Multisample;
	Transform<Capability::SampleShading>
		SampleShading;
	Transform<Capability::LineSmooth>
		LineSmooth;
	Transform<Capability::PolygonSmooth>
		PolygonSmooth;
	Transform<Capability::ProgramPointSize>
		ProgramPointSize;
	Transform<Capability::TextureCubeMapSeamless>
		TextureCubeMapSeamless;
	Transform<Capability::SampleAlphaToCoverage>
		SampleAlphaToCoverage;
	Transform<Capability::SampleAlphaToOne>
		SampleAlphaToOne;
	Transform<Capability::SampleCoverage>
		SampleCoverage;
	Transform<Capability::SampleMask>
		SampleMask;
	Transform<Capability::FramebufferSRGB>
		FramebufferSRGB;
	Transform<Capability::DebugOutputSynchronous>
		DebugOutputSynchronous;
	Transform<Capability::StreamRasterization>
		StreamRasterization;
	Transform<Capability::BlendAdvancedCoherent>
		BlendAdvancedCoherent;
	Transform<Capability::FragmentCoverageToColor>
		FragmentCoverageToColor;
};

} // namespace enums
#endif
//]

