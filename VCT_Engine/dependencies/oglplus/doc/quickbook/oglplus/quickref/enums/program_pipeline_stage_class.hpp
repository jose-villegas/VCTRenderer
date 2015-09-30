//  File doc/quickbook/oglplus/quickref/enums/program_pipeline_stage_class.hpp
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
//[oglplus_enums_program_pipeline_stage_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ProgramPipelineStage> class Transform>
class __EnumToClass<Base, __ProgramPipelineStage, Transform> /*<
Specialization of __EnumToClass for the __ProgramPipelineStage enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ProgramPipelineStage::VertexShader>
		VertexShader;
	Transform<ProgramPipelineStage::TessControlShader>
		TessControlShader;
	Transform<ProgramPipelineStage::TessEvaluationShader>
		TessEvaluationShader;
	Transform<ProgramPipelineStage::GeometryShader>
		GeometryShader;
	Transform<ProgramPipelineStage::FragmentShader>
		FragmentShader;
	Transform<ProgramPipelineStage::ComputeShader>
		ComputeShader;
	Transform<ProgramPipelineStage::AllShaders>
		AllShaders;
};

} // namespace enums
#endif
//]

