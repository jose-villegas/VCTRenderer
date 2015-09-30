/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_shader_common

template <>
struct __ObjectSubtype<__tag_Shader>
{
	typedef __ShaderType Type;
};

template <>
class __ObjCommonOps<__tag_Shader>
 : public __ShaderName
{
public:
#if GL_ES_VERSION_3_0 || GL_VERSION_4_1 || GL_ARB_ES2_compatibility
	static void PrecisionFormat(
		__ShaderType shader_type,
		__PrecisionType precision_type,
		GLint* range_log_2,
		GLint* precision_log_2
	); /*< Gets the shader precision format.
	See [glfunc GetShaderPrecisionFormat].
	>*/
};
//]
//[oglplus_shader_1
template <>
class __ObjectOps<__tag_DirectState, __tag_Shader>
 : public __ObjZeroOps<__tag_DirectState, __tag_Shader> /*<
Indirectly inherits from __ObjCommonOps_Shader<__tag_Shader>.
>*/
{
public:
	struct Property
	{
		typedef __ShaderType Type;
	};

	__ShaderType Type(void) const; /*<
	Returns the type of [^this] shader.
	See [glfunc GetShader], [glconst SHADER_TYPE].
	>*/

	__ObjectOps& Source(
		const __SizeType count,
		const GLchar* const * srcs,
		const GLint* lens
	); /*<
	Set the source code of this shader.
	See [glfunc ShaderSource].
	>*/
	ObjectOps& Source(__GLSLString&& source);
	ObjectOps& Source(__GLSLStrings&& source);
	ObjectOps& Source(const __GLSLSource& glsl_source);

	__Boolean IsCompiled(void) const; /*<
	Returns true if the shader is already compiled, returns false otherwise.
	See [glfunc GetShader], [glconst COMPILE_STATUS].
	>*/

	ObjectOps& Compile(void); /*<
	Compiles [^this] shader.
	Throws __CompileError if the shader cannot be compiled.
	See [glfunc CompileShader].
	>*/
	__Outcome<ObjectOps&> Compile(std::nothrow_t); /*<
	This overload of [^Compile] defers the error handling.
	>*/

	__String GetInfoLog(void) const; /*<
	Returns the compiler output if the shader is compiled.
	See [glfunc GetShader], [glfunc GetShaderInfoLog].
	>*/

#if GL_ARB_shading_language_include
	ObjectOps& CompileInclude(
		__SizeType count,
		const GLchar* const* paths,
		const GLint* lengths
	); /*<
	Compiles the shader using the specified include paths.
	See [glfunc CompileShaderIncludeARB].
	>*/

	ObjectOps& CompileInclude(__GLSLString&& incl);
	ObjectOps& CompileInclude(__GLSLStrings&& incl);
	ObjectOps& CompileInclude(const __GLSLSource& incl);

	__Outcome<ObjectOps&> CompileInclude(
		__SizeType count,
		const GLchar* const* paths,
		const GLint* lengths,
		std::nothrow_t
	);

	__Outcome<ObjectOps&> CompileInclude(__GLSLString&& incl, std::nothrow_t);
	Outcome<ObjectOps&> CompileInclude(__GLSLStrings&& incl, std::nothrow_t);
	Outcome<ObjectOps&> CompileInclude(const __GLSLSource& incl, std::nothrow_t);
#endif

#if GL_ES_VERSION_3_0 || GL_VERSION_4_1 || GL_ARB_ES2_compatibility
	static void ReleaseCompiler(void); /*<
	Indicates that the resources associated with the compiler can be freed.
	See [glfunc ReleaseShaderCompiler].
	>*/
#endif
};

//]
//[oglplus_shader_2

typedef __ObjectOps<__tag_DirectState, __tag_Shader> ShaderOps;

class Shader
 : public __Object<ShaderOps>
{
public:
	Shader(__ShaderType type);

	Shader(__ShaderType type, __ObjectDesc&& description);

	Shader(__ShaderType type, __GLSLString&& glsl_source);
	Shader(__ShaderType type, __GLSLStrings&& glsl_source);
	Shader(__ShaderType type, const __GLSLSource& glsl_source);

	Shader(
		__ShaderType type,
		__ObjectDesc&& description,
		__GLSLString&& glsl_source
	);

	Shader(
		__ShaderType type,
		__ObjectDesc&& description,
		__GLSLStrings&& glsl_source
	);

	Shader(
		__ShaderType type,
		__ObjectDesc&& description,
		const __GLSLSource& glsl_source
	);

	Shader(const Shader&) = delete;
	Shader(Shader&&);

	Shader& operator = (Shader&&);
};

//]
//[oglplus_spec_shader_1

template <__ShaderType Type>
class SpecShader
 : public __Shader
{
public:
	SpecShader(void);

	SpecShader(__ObjectDesc&& description);

	SpecShader(__GLSLString&& glsl_source);
	SpecShader(__GLSLStrings&& glsl_source);
	SpecShader(const __GLSLSource& glsl_source);

	SpecShader(__ObjectDesc&& description, __GLSLString&& glsl_source);
	SpecShader(__ObjectDesc&& description, __GLSLStrings&& glsl_source);
	SpecShader(__ObjectDesc&& description, const __GLSLSource& glsl_source);

	SpecShader(const SpecShader&) = delete;
	SpecShader(SpecShader&&);
};

typedef __SpecShader<__ShaderType::Vertex> VertexShader;

#if GL_TESS_CONTROL_SHADER
typedef __SpecShader<__ShaderType::TessControl> TessControlShader;
#endif

#if GL_TESS_EVALUATION_SHADER
typedef SpecShader<__ShaderType::TessEvaluation> TessEvaluationShader;
#endif

#if GL_GEOMETRY_SHADER
typedef __SpecShader<__ShaderType::Geometry> GeometryShader;
#endif

typedef __SpecShader<__ShaderType::Fragment> FragmentShader;

#if GL_COMPUTE_SHADER
typedef __SpecShader<__ShaderType::Compute> ComputeShader;
#endif

//]

