/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_program_common

template <>
class __ObjCommonOps<__tag_Program>
 : public __ProgramName
{
public:
	static __ProgramName Binding(void); /*<
	Returns the currently bound program.
	See [glfunc Get], [glconst PROGRAM_BINDING].
	>*/

	static void Bind(__ProgramName program); /*<
	Binds the specified [^program] object.
	See [glfunc BindProgram].
	>*/
	void Bind(void) const; /*<
	Binds [^this] program object.
	See [glfunc BindProgram].
	>*/

	void Use(void); /*<
	Makes [^this] program current.
	See [glfunc UseProgram].
	>*/
};
//]

//[oglplus_program_1
template <>
class __ObjectOps<__tag_DirectState, __tag_Program>
 : public __ObjZeroOps<__tag_DirectState, __tag_Program>
{
public:
	ObjectOps& AttachShader(__ShaderName shader); /*<
	Attaches a shader to [^this] program.
	See [glfunc AttachShader].
	>*/

	ObjectOps& AttachShaders(const __Sequence<__ShaderName>& shaders); /*<
	Attaches a set of shaders to [^this] program.
	See [glfunc AttachShader].
	>*/

	ObjectOps& DetachShader(__ShaderName shader); /*<
	Detaches a shader from [^this] program.
	See [glfunc DetachShader].
	>*/

	__Boolean IsLinked(void) const; /*<
	Returns true if [^this] program is already linked, false otherwise.
	See [glfunc GetProgram], [glconst LINK_STATUS].
	>*/

	__String GetInfoLog(void) const; /*<
	Returns the linker output if the program is linked.
	See [glfunc GetProgramInfoLog].
	>*/

	ObjectOps& Link(void); /*<
	Links this shading language program.
	Throws __LinkError if the program cannot be linked.
	See [glfunc LinkProgram].
	>*/
	__Outcome<ObjectOps&> Link(std::nothrow_t);

	__Outcome<ObjectOps&> Build(void); /*<
	Checks if all attached shaders are compiled
	and if they are not the it compiles them and then links
	this __Program.
	Throws __CompileError if any of the shaders cannot be compiled,
	throws __LinkError if the program cannot be linked.
	See [glfunc CompileShader], [glfunc LinkProgram].
	>*/

#if GL_ARB_shading_language_include
	__Outcome<ObjectOps&> BuildInclude(
		__SizeType count,
		const GLchar* const* paths,
		const GLint* lengths
	); /*<
	Checks if all attached shaders are compiled
	and if they are not the it compiles them and using the specified
	include paths and then links [^this] __Program.
	See [glfunc CompileShader], [glfunc LinkProgram].
	>*/
	__Outcome<ObjectOps&> BuildInclude(__GLSLString&& incl);
	Outcome<ObjectOps&> BuildInclude(__GLSLStrings&& incl);
	Outcome<ObjectOps&> BuildInclude(const __GLSLSource&& incl);
#endif

	__Boolean IsValid(void) const; /*<
	Returns true if [^this] program is validated, false otherwise.
	See [glfunc GetProgram], [glconst VALIDATE_STATUS].
	>*/

	ObjectOps& Validate(void); /*<
	Validates [^this] shading language program.
	Throws __ValidationError if the program is not valid.
	See [glfunc ValidateProgram].
	>*/
	__Outcome<ObjectOps&> Validate(std::nothrow_t);
//]
//[oglplus_program_2

	void TransformFeedbackVarying(const GLchar* varying); /*<
	Sets the variables that will be captured during transform feedback
	See [glfunc TransformFeedbackVaryings].
	>*/

	void TransformFeedbackVaryings(
		__SizeType count,
		const GLchar** varyings,
		__TransformFeedbackMode mode
	); /*<
	Sets the variables that will be captured during transform feedback
	See [glfunc TransformFeedbackVaryings].
	>*/

	template <typename std::size_t N>
	void TransformFeedbackVaryings(
		const GLchar* (&varyings)[N],
		__TransformFeedbackMode mode
	);

	void TransformFeedbackVaryings(
		const std::vector<__String>& varyings,
		__TransformFeedbackMode mode
	) const;


#if GL_VERSION_4_1 || GL_ARB_separate_shader_objects
	ObjectOps& MakeSeparable(__Boolean para = true); /*<
	Makes this program separable (or non-separable).
	See [glfunc ProgramParameter].
	>*/
#endif

#if GL_VERSION_4_1 || GL_ARB_get_program_binary
	ObjectOps& MakeRetrievable(__Boolean para = true); /*<
	Makes this program retrievable (or non-retrievable) in binary form.
	See [glfunc ProgramParameter].
	>*/

	void GetBinary(std::vector<GLubyte>& binary, GLenum& format) const; /*<
	Gets the program code in binary form.
	See [glfunc GetProgramBinary].
	>*/

	void Binary(const std::vector<GLubyte>& binary, GLenum format); /*<
	Specifies the program code in binary form.
	See [glfunc ProgramBinary].
	>*/
#endif

	typedef __Unspecified InterfaceContext; /*<
	Helper class for efficient iteration of [^Program] interface items.
	Instances of this class are created by a program for its specific
	interfaces (uniform, vertex attributes, subroutines, etc.) or
	stages (vertex, geometry, fragment, etc.). Instances of an interface
	context can be used (mostly internally) for efficient iteration
	of individual items of a particular interface (uniforms, subroutines,
	etc.). Contexts for various programs and various interfaces are not
	interchangeable.
	The InterfaceContext type should be treated as opaque and only used
	with appropriate functions.
	>*/

#if GL_VERSION_4_3
	typedef __Range<__ProgramResource> ActiveResourceRange; /*<
	The type of the range for traversing program resource information.
	>*/

	InterfaceContext ActiveResourceContext(__ProgramInterface intf) const; /*<
	Returns the context for traversal of Program's active resources.
	>*/

	ActiveResourceRange ActiveResources(__ProgramInterface intf) const; /*<
	Returns a range allowing to do the traversal of interface's resources.
	This instance of [^Program] must be kept alive during the whole
	lifetime of the returned range, i.e. the returned range must not
	be used after the [^Program] goes out of scope and is destroyed.
	>*/
#endif

//]
//[oglplus_program_3
	class ActiveVariableInfo /*<
	Class providing information about a single active vertex attribute
	or uniform.
	Note that the [^Program]'s functions documented as returning instances
	of [^ActiveVariableInfo] actually return types convertible to
	[^ActiveVariableInfo].
	>*/
	{
	public:
		GLuint Index(void) const; /*<
		Returns the index of the attribute or uniform.
		>*/

		const __String& Name(void) const; /*<
		Returns the name (identifier) of the attribute or uniform.
		>*/

		const GLint Size(void) const; /*<
		Returns the size in units of [^Type].
		>*/

		const __SLDataType Type(void) const; /*<
		Returns the data type of the variable.
		>*/
	};

	typedef __Range<ActiveVariableInfo> ActiveAttribRange; /*<
	The type of the range for traversing active vertex attributes.
	>*/

	InterfaceContext ActiveAttribContext(void) const; /*<
	Returns the context for traversal of [^Program]'s active vertex attributes.
	>*/

	ActiveAttribRange ActiveAttribs(void) const; /*<
	Returns a range allowing to do the traversal of active attributes.
	This instance of [^Program] must be kept alive during the whole
	lifetime of the returned range, i.e. the returned range must not
	be used after the [^Program] goes out of scope and is destroyed.
	>*/


	typedef __Range<ActiveVariableInfo> ActiveUniformRange; /*<
	The type of the range for traversing active uniforms.
	>*/

	InterfaceContext ActiveUniformContext(void) const; /*<
	Returns the context for traversal of Program's active uniforms.
	>*/

	ActiveUniformRange ActiveUniforms(void) const; /*<
	Returns a range allowing to do the traversal of active uniforms.
	This instance of [^Program] must be kept alive during the whole
	lifetime of the returned range, i.e. the returned range must not
	be used after the [^Program] goes out of scope and is destroyed.
>*/
//]
//[oglplus_program_4

#if GL_VERSION_4_0 || GL_ARB_shader_subroutine
	typedef __Range<ActiveVariableInfo> ActiveSubroutineRange; /*<
	The type of the range for traversing active subroutines.
	>*/

	InterfaceContext ActiveSubroutineContext(ShaderType stage) const; /*<
	Returns the context for traversal of Program's active subroutines.
	>*/

	ActiveSubroutineRange ActiveSubroutines(ShaderType stage) const; /*<
	Returns a range allowing to do the traversal of subroutines.
	This instance of [^Program] must be kept alive during the whole
	lifetime of the returned range, i.e. the returned range must not
	be used after the [^Program] goes out of scope and is destroyed.
	>*/

	typedef __Range<ActiveVariableInfo> ActiveSubroutineUniformRange; /*<
	The type of the range for traversing active subroutine uniforms.
	>*/

	InterfaceContext ActiveSubroutineUniformContext(ShaderType stage) const; /*<
	Returns the context for traversal of Program's active subroutine uniforms.
	>*/

	ActiveSubroutineUniformRange ActiveSubroutineUniforms(ShaderType stage) const; /*<
	Returns a range allowing to do the traversal of subroutine uniforms.
	This instance of [^Program] must be kept alive during the whole
	lifetime of the returned range, i.e. the returned range must not
	be used after the [^Program] goes out of scope and is destroyed.
	>*/
#endif

	typedef __Range<ActiveVariableInfo> TransformFeedbackVaryingRange; /*<
	The type of the range for traversing transform feedback varyings.
	>*/

	InterfaceContext TransformFeedbackVaryingContext(void) const; /*<
	Returns the context for traversal of Program's active transform
	feedback varyings.
	>*/

	TransformFeedbackVaryingRange TransformFeedbackVaryings(void) const; /*<
	Returns a range allowing to do the traversal of transform feedback varyings.
	This instance of [^Program] must be kept alive during the whole
	lifetime of the returned range, i.e. the returned range must not
	be used after the [^Program] goes out of scope and is destroyed.
	>*/


	ShaderRange AttachedShaders(void) const; /*<
	Returns a range allowing to traverse shaders attached to [^this] program.
	>*/
//]
//[oglplus_program_5

	__TransformFeedbackMode TransformFeedbackBufferMode(void) const; /*<
	Returns the transform feedback buffer mode.
	See [glfunc GetProgram], [glconst TRANSFORM_FEEDBACK_BUFFER_MODE].
	>*/

#if GL_VERSION_3_2
	GLint GeometryVerticesOut(void) const; /*<
	Returns the number of vertices that the geometry shader will output.
	See [glfunc GetProgram], [glconst GEOMETRY_VERTICES_OUT].
	>*/

	__PrimitiveType GeometryInputType(void) const; /*<
	Returns the geometry shader input primitive type
	See [glfunc GetProgram], [glconst GEOMETRY_INPUT_TYPE].
	>*/
	__PrimitiveType GeometryOutputType(void) const; /*<
	Returns the geometry shader output primitive type
	See [glfunc GetProgram], [glconst GEOMETRY_OUTPUT_TYPE].
	>*/
#endif

#if GL_VERSION_4_1 || GL_ARB_gpu_shader5
	GLint GeometryShaderInvocations(void) const; /*<
	Returns the number of invocations of geometry shader per primitive.
	See [glfunc GetProgram], [glconst GEOMETRY_SHADER_INVOCATIONS].
	>*/
#endif

#if GL_VERSION_4_0 || GL_ARB_tessellation_shader
	__FaceOrientation TessGenVertexOrder(void) const; /*<
	Returns the vertex order in tesselation evaluation shader.
	See [glfunc GetProgram], [glconst TESS_GEN_VERTEX_ORDER].
	>*/

	__TessGenPrimitiveType TessGenMode(void) const; /*<
	Returns the tesselation generator output primitive type.
	See [glfunc GetProgram], [glconst TESS_GEN_MODE].
	>*/

	__TessGenPrimitiveSpacing TessGenSpacing(void) const; /*<
	Returns the tesselation generator primitive spacing mode.
	See [glfunc GetProgram], [glconst TESS_GEN_SPACING].
	>*/

	__Boolean TessGenPointMode(void) const; /*<
	Returns true if point mode is enabled in tesslation eval. shader.
	See [glfunc GetProgram], [glconst TESS_GEN_POINT_MODE].
	>*/
#endif

	void BindLocation(
		__VertexAttribSlot vertex_attrib_slot,
		__StrCRef identifier
	); /*<
	Binds the location of a shading language variable to a vertex
	attribute.
	>*/
};
//]
//[oglplus_program_def

typedef __ObjectOps<__tag_DirectState, __tag_Program>
	ProgramOps;

typedef __ObjectZero<__ObjZeroOps<__tag_DirectState, __tag_Program>>
	NoProgram;

typedef __Object<ProgramOps>
	Program;

//]
//[oglplus_program_sugar

ProgramOps& operator << (
	ProgramOps& program,
	__ShaderName shader
); /*<
Equivalent to `program.AttachShader(shader)`.
>*/

struct ProgAndXFBMode
{
	ProgAndXFBMode(ProgramOps& p, __TransformFeedbackMode m);
};

ProgAndXFBMode operator << (
	ProgramOps& prog,
	__TransformFeedbackMode mode
);

template <std::size_t N>
ProgramOps& operator << (
	ProgAndXFBMode pam,
	const GLchar* (&varyings)[N]
);

struct ProgXFBModeAndNames
{
	ProgXFBModeAndNames(ProgAndXFBMode pam, const GLchar* name);

	ProgXFBModeAndNames(ProgXFBModeAndNames&& pman, const GLchar* name);

	ProgXFBModeAndNames(ProgXFBModeAndNames&& tmp);
};

ProgXFBModeAndNames operator << (
	ProgAndXFBMode pam,
	const GLchar* name
);

ProgXFBModeAndNames operator << (
	ProgXFBModeAndNames&& pman,
	const GLchar* name
);


//]
//[oglplus_shader_program

#if GL_VERSION_4_1 || GL_ARB_separate_shader_objects
class ShaderProgram /*<
See [glfunc CreateShaderProgram].
>*/
 : public __Program
{
public:
	ShaderProgram(
		__ShaderType shader_type,
		__GLSLString&& source
	); /*<
	Creates a program with a single shader with specified type and source.
	Throws __ValidationError.
	>*/

	ShaderProgram(
		__ShaderType shader_type,
		__GLSLString&& source,
		__ObjectDesc&& object_desc
	); /*<
	Creates a program with a single shader with specified type, source
	and description.
	Throws __ValidationError.
	>*/

	ShaderProgram(
		__ShaderType shader_type,
		__GLSLStrings&& source
	); /*<
	Creates a program with a single shader with specified type and source.
	Throws __ValidationError.
	>*/

	ShaderProgram(
		__ShaderType shader_type,
		__GLSLStrings&& source,
		__ObjectDesc&& object_desc
	); /*<
	Creates a program with a single shader with specified type, source
	and description.
	Throws __ValidationError.
	>*/

	ShaderProgram(
		__ShaderType shader_type,
		const __GLSLSource& glsl_source
	); /*<
	Creates a program with a single shader with specified type and source.
	Throws __ValidationError.
	>*/

	ShaderProgram(
		__ShaderType shader_type,
		const __GLSLSource& glsl_source,
		__ObjectDesc&& object_desc
	); /*<
	Creates a program with a single shader with specified type, source
	and description.
	Throws __ValidationError.
	>*/
};
#endif

//]
//[oglplus_quick_program

class QuickProgram
 : public __Program
{
public:
	QuickProgram(const __Sequence<__ShaderName>& shaders); /*<
	Attaches [^shaders], links and uses the program.
	>*/

	QuickProgram(
		__ObjectDesc&& object_desc,
		const __Sequence<__ShaderName>& shaders
	); /*<
	Attaches [^shaders], links, uses and describes the program.
	>*/

#if GL_VERSION_4_1 || GL_ARB_separate_shader_objects
	QuickProgram(bool separable, const __Sequence<__ShaderName>& shaders); /*<
	Attaches [^shaders], makes the program separable, links and uses it.
	>*/

	QuickProgram(
		__ObjectDesc&& object_desc,
		bool separable,
		const __Sequence<__ShaderName>& shaders
	); /*<
	Attaches [^shaders], makes the program separable, links, uses
	and describes it.
	>*/
#endif
};

//]

