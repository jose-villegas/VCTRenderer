//  File doc/quickbook/oglplus/quickref/enums/program_resource_property_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/program_resource_property.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_program_resource_property_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ProgramResourceProperty> class Transform>
class __EnumToClass<Base, __ProgramResourceProperty, Transform> /*<
Specialization of __EnumToClass for the __ProgramResourceProperty enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ProgramResourceProperty::ArraySize>
		ArraySize;
	Transform<ProgramResourceProperty::Offset>
		Offset;
	Transform<ProgramResourceProperty::BlockIndex>
		BlockIndex;
	Transform<ProgramResourceProperty::ArrayStride>
		ArrayStride;
	Transform<ProgramResourceProperty::MatrixStride>
		MatrixStride;
	Transform<ProgramResourceProperty::IsRowMajor>
		IsRowMajor;
	Transform<ProgramResourceProperty::AtomicCounterBufferIndex>
		AtomicCounterBufferIndex;
	Transform<ProgramResourceProperty::BufferBinding>
		BufferBinding;
	Transform<ProgramResourceProperty::BufferDataSize>
		BufferDataSize;
	Transform<ProgramResourceProperty::NumActiveVariables>
		NumActiveVariables;
	Transform<ProgramResourceProperty::ActiveVariables>
		ActiveVariables;
	Transform<ProgramResourceProperty::ReferencedByVertexShader>
		ReferencedByVertexShader;
	Transform<ProgramResourceProperty::ReferencedByTessControlShader>
		ReferencedByTessControlShader;
	Transform<ProgramResourceProperty::ReferencedByTessEvaluationShader>
		ReferencedByTessEvaluationShader;
	Transform<ProgramResourceProperty::ReferencedByGeometryShader>
		ReferencedByGeometryShader;
	Transform<ProgramResourceProperty::ReferencedByFragmentShader>
		ReferencedByFragmentShader;
	Transform<ProgramResourceProperty::ReferencedByComputeShader>
		ReferencedByComputeShader;
	Transform<ProgramResourceProperty::NumCompatibleSubroutines>
		NumCompatibleSubroutines;
	Transform<ProgramResourceProperty::CompatibleSubroutines>
		CompatibleSubroutines;
	Transform<ProgramResourceProperty::TopLevelArraySize>
		TopLevelArraySize;
	Transform<ProgramResourceProperty::TopLevelArrayStride>
		TopLevelArrayStride;
	Transform<ProgramResourceProperty::Location>
		Location;
	Transform<ProgramResourceProperty::LocationIndex>
		LocationIndex;
	Transform<ProgramResourceProperty::LocationComponent>
		LocationComponent;
	Transform<ProgramResourceProperty::IsPerPatch>
		IsPerPatch;
	Transform<ProgramResourceProperty::TransformFeedbackBufferIndex>
		TransformFeedbackBufferIndex;
	Transform<ProgramResourceProperty::TransformFeedbackBufferStride>
		TransformFeedbackBufferStride;
};

} // namespace enums
#endif
//]

