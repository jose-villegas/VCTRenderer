/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_uniform_block_loc_ops

template <>
class __ProgVarLocOps<__tag_UniformBlock>
{
public:
	static GLint GetLocation(
		__ProgramName program,
		__StrCRef identifier,
		bool active_only
	); /*<
	Finds the location of the uniform block specified
	by [^identifier] in a [^program]. If [^active_only] is [^true] then
	throws __ProgVarError if no such uniform block exists or if it
	is not active.
	See [glfunc GetUniformBlockIndex].
	>*/
};

//]
//[oglplus_uniform_block_common_ops

template <>
class __ProgVarCommonOps<__tag_UniformBlock>
 : public __ProgVarLoc<__tag_UniformBlock>
{
public:
	static GLuint MaxIn(__ShaderType shader_type); /*<
	Returns the maximum number of uniform blocks for a [^shader_type].
	>*/

	__Boolean ReferencedBy(__ShaderType shader_type) const; /*<
	Returns [^true] if this uniform block is referenced by a [^shader_type].
	See [glfunc GetActiveUniformBlock].
	>*/

	GLuint DataSize(void) const; /*<
	Returns the size of the uniform block.
	See [glfunc GetActiveUniformBlock], [glconst UNIFORM_BLOCK_DATA_SIZE].
	>*/

	void Binding(__UniformBufferBindingPoint binding); /*<
	Sets up the uniform block binding.
	See [glfunc UniformBlockBinding].
	>*/
};

//]
//[oglplus_uniform_block_def

template <typename T>
using Uniform = __ProgVar<
	__tag_ImplicitSel,
	__tag_UniformBlock,
	__tag_NoTypecheck,
	void
>; /*<
[^UniformBlock] indirectly inherits from __ProgVarLocOps_UniformBlock and __ProgVarCommonOps_Uniform.
>*/

//]

