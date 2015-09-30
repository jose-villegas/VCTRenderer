//  File doc/quickbook/oglplus/quickref/enums/program_interface_class.hpp
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
//[oglplus_enums_program_interface_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ProgramInterface> class Transform>
class __EnumToClass<Base, __ProgramInterface, Transform> /*<
Specialization of __EnumToClass for the __ProgramInterface enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ProgramInterface::Uniform>
		Uniform;
	Transform<ProgramInterface::UniformBlock>
		UniformBlock;
	Transform<ProgramInterface::AtomicCounterBuffer>
		AtomicCounterBuffer;
	Transform<ProgramInterface::ProgramInput>
		ProgramInput;
	Transform<ProgramInterface::ProgramOutput>
		ProgramOutput;
	Transform<ProgramInterface::VertexSubroutine>
		VertexSubroutine;
	Transform<ProgramInterface::TessControlSubroutine>
		TessControlSubroutine;
	Transform<ProgramInterface::TessEvaluationSubroutine>
		TessEvaluationSubroutine;
	Transform<ProgramInterface::GeometrySubroutine>
		GeometrySubroutine;
	Transform<ProgramInterface::FragmentSubroutine>
		FragmentSubroutine;
	Transform<ProgramInterface::ComputeSubroutine>
		ComputeSubroutine;
	Transform<ProgramInterface::VertexSubroutineUniform>
		VertexSubroutineUniform;
	Transform<ProgramInterface::TessControlSubroutineUniform>
		TessControlSubroutineUniform;
	Transform<ProgramInterface::TessEvaluationSubroutineUniform>
		TessEvaluationSubroutineUniform;
	Transform<ProgramInterface::GeometrySubroutineUniform>
		GeometrySubroutineUniform;
	Transform<ProgramInterface::FragmentSubroutineUniform>
		FragmentSubroutineUniform;
	Transform<ProgramInterface::ComputeSubroutineUniform>
		ComputeSubroutineUniform;
	Transform<ProgramInterface::TransformFeedebackVarying>
		TransformFeedebackVarying;
	Transform<ProgramInterface::BufferVariable>
		BufferVariable;
	Transform<ProgramInterface::ShaderStorageBlock>
		ShaderStorageBlock;
};

} // namespace enums
#endif
//]

