/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_program_pipeline_common

template <>
class __ObjCommonOps<__tag_ProgramPipeline>
 : public __ProgramPipelineName
{
public:
	static __ProgramPipelineName Binding(void); /*<
	Returns the currently bound program pipeline.
	See [glfunc GetIntegerv].
	>*/

	static void Bind(__ProgramPipelineName pipeline); /*<
	Binds the specified program pipeline
	See [glfunc BindProgramPipeline].
	>*/

	void Bind(void) const; /*<
	Binds [^this] program pipeline.
	See [glfunc BindProgramPipeline].
	>*/
};
//]
//[oglplus_program_pipeline_1
template <>
class __ObjectOps<__tag_DirectState, __tag_ProgramPipeline>
 : public __ObjZeroOps<__tag_DirectState, __tag_ProgramPipeline> /*<
Indirectly inherits from __ObjCommonOps_ProgramPipeline<__tag_ProgramPipeline>.
>*/
{
public:
	struct Properties
	{
		typedef __ProgramPipelineStage Stage;
	};

	ProgPLUseStages UseStages(__ProgramName program) const; /*<
	This function returns an object that allows to specify which stages
	of [^program] should by used when this pipeline is active by calling
	its [^Vertex()], [^TessControl()], [^TessEvaluation()], [^Geometry()],
	[^Fragment()] and and [^All()] member functions of the object returned
	by [^UseStages].
	See [glfunc UseProgramStages].

	``
	__Program prog;
	__ProgramPipeline pp;
	...
	pp.UseStages(prog).Vertex();
	pp.UseStages(prog).Vertex().Geometry();
	pp.UseStages(prog).Vertex().TessControl().TessEvaluation().Geometry();
	pp.UseStages(prog).Vertex().Geometry().Fragment();
	pp.UseStages(prog).Geometry();
	pp.UseStages(prog).All();
	``
	>*/

	void UseStages(
		__Bitfield<__ProgramPipelineStage> stages,
		__ProgramName program
	) const; /*<
	Uses the specified [^stages] of the specified [^program].
	See [glfunc UseProgramStages].
	>*/

#if defined GL_ALL_SHADER_BITS
	void UseAllStages(__ProgramName program) const; /*<
	Use all stages of the [^program].
	See [glfunc UseProgramStages].
	>*/
#endif

	__String GetInfoLog(void) const; /*<
	Returns the validation process output.
	See [glfunc GetProgramPipeline], [glfunc GetProgramPipelineInfoLog].
	>*/

	__Boolean IsValid(void) const; /*<
	Returns true if the pipeline is validated, false otherwise.
	See [glfunc GetProgramPipeline], [glconst VALIDATE_STATUS].
	>*/

	ObjectOps& Validate(void); /*<
	Validates this program pipeline.
	Throws __ValidationError on failure.
	See [glfunc ValidateProgramPipeline].
	>*/

	__Outcome<ObjectOps&> Validate(std::nothrow_t);

	void ActiveShaderProgram(__ProgramName program) const; /*<
	Make the [^program] active for this program pipeline.
	See [glfunc ActiveShaderProgram].
	>*/
	__ProgramName ActiveShaderProgram(void) const; /*<
	Returns the current active shader program.
	See [glfunc GetProgramPipeline], [glconst ACTIVE_PROGRAM].
	>*/

	__Boolean HasShader(__ShaderType shader_type) const; /*<
	Returns true if this pipeline contains a shader of the specified type.
	See [glfunc GetProgramPipeline].
	>*/

	__ProgramName ShaderProgram(__ShaderType shader_type) const; /*<
	Returns the program from which the shader of the specified type is used.
	See [glfunc GetProgramPipeline].
	>*/
};
//]
//[oglplus_program_pipeline_def

typedef ObjectOps<__tag_DirectState, __tag_ProgramPipeline>
	ProgramPipelineOps;

typedef __Object<ProgramPipelineOps> ProgramPipeline;

typedef __ObjectZero<__ObjZeroOps<__tag_DirectState, __tag_ProgramPipeline>> /*<
Indirectly inherits from __ObjCommonOps_ProgramPipeline<__tag_ProgramPipeline>.
>*/
	NoProgramPipeline;

//]

