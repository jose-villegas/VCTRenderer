/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_uniform_loc_ops

template <>
class __ProgVarLocOps<__tag_Uniform>
{
public:
	static GLint GetLocation(
		__ProgramName program,
		__StrCRef identifier,
		bool active_only
	); /*<
	Finds the location of the uniform variable specified
	by [^identifier] in a [^program]. If [^active_only] is [^true] then
	throws __ProgVarError if no such uniform exists or if it is not active.
	See [glfunc GetUniformLocation].
	>*/
};

//]
//[oglplus_uniform_get_set_ops

template <typename __OpsTag, typename T>
class __ProgVarGetSetOps<__OpsTag, __tag_Uniform, T>
 : public __ProgVarCommonOps<__tag_Uniform>
{
public:
	void Set(T value);
	void SetValue(T value); /*<
	Sets uniform value.
	>*/

	void SetValues(std::size_t n, const T* values); /*<
	Sets multiple consecutive values.
	>*/

	void TrySet(T value); /*<
	Sets the uniform value if it is active.
	See [glfunc VertexAttrib].
	>*/
};

template <typename __OpsTag>
class __ProgVarGetSetOps<__OpsTag, __tag_Uniform, void>
 : public __ProgVarCommonOps<__tag_Uniform>
{
public:
	template <typename T>
	void Set(T value);

	template <typename T>
	void SetValue(T value); /*<
	Sets uniform value.
	>*/

	template <typename T>
	void SetValues(std::size_t n, const T* values); /*<
	Sets multiple consecutive values.
	>*/
};

//]
//[oglplus_uniform_def

template <typename T>
using Uniform = __ProgVar<
	__tag_ImplicitSel,
	__tag_Uniform,
	__tag_NoTypecheck,
	T
>; /*<
[^Uniform] indirectly inherits from __ProgVarLocOps_Uniform and __ProgVarGetSetOps_Uniform.
>*/

typedef __Uniform<GLint> UniformSampler;

//]
//[oglplus_untyped_uniform

typedef __ProgVar<
	__tag_ImplicitSel,
	__tag_Uniform,
	__tag_NoTypecheck,
	void
> UntypedUniform; /*<
[^Uniform] indirectly inherits from __ProgVarLocOps_Uniform and __ProgVarGetSetOps_Uniform.
>*/

UntypedUniform
operator / (__ProgramName program, __StrCRef identifier);

//]

