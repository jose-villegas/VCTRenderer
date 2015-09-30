//  File doc/quickbook/oglplus/quickref/enums/query_target_class.hpp
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
//[oglplus_enums_query_target_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__QueryTarget> class Transform>
class __EnumToClass<Base, __QueryTarget, Transform> /*<
Specialization of __EnumToClass for the __QueryTarget enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<QueryTarget::TimeElapsed>
		TimeElapsed;
	Transform<QueryTarget::Timestamp>
		Timestamp;
	Transform<QueryTarget::SamplesPassed>
		SamplesPassed;
	Transform<QueryTarget::AnySamplesPassed>
		AnySamplesPassed;
	Transform<QueryTarget::PrimitivesGenerated>
		PrimitivesGenerated;
	Transform<QueryTarget::TransformFeedbackPrimitivesWritten>
		TransformFeedbackPrimitivesWritten;
	Transform<QueryTarget::VerticesSubmitted>
		VerticesSubmitted;
	Transform<QueryTarget::PrimitivesSubmitted>
		PrimitivesSubmitted;
	Transform<QueryTarget::VertexShaderInvocations>
		VertexShaderInvocations;
	Transform<QueryTarget::TessControlShaderPatches>
		TessControlShaderPatches;
	Transform<QueryTarget::TessEvaluationShaderInvocations>
		TessEvaluationShaderInvocations;
	Transform<QueryTarget::GeometryShaderInvocations>
		GeometryShaderInvocations;
	Transform<QueryTarget::GeometryShaderPrimitivesEmitted>
		GeometryShaderPrimitivesEmitted;
	Transform<QueryTarget::FragmentShaderInvocations>
		FragmentShaderInvocations;
	Transform<QueryTarget::ComputeShaderInvocations>
		ComputeShaderInvocations;
	Transform<QueryTarget::ClippingInputPrimitives>
		ClippingInputPrimitives;
	Transform<QueryTarget::ClippingOutputPrimitives>
		ClippingOutputPrimitives;
	Transform<QueryTarget::TransformFeedbackOverflow>
		TransformFeedbackOverflow;
	Transform<QueryTarget::TransformFeedbackStreamOverflow>
		TransformFeedbackStreamOverflow;
};

} // namespace enums
#endif
//]

