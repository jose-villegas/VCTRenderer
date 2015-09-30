/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_shader_storage_block_loc_ops

template <>
class __ProgVarLocOps<__tag_ShaderStorageBlock>
{
public:
	static GLint GetLocation(
		__ProgramName program,
		__StrCRef identifier,
		bool active_only
	); /*<
	Finds the location of the shader storage block specified
	by [^identifier] in a [^program]. If [^active_only] is [^true] then
	throws __ProgVarError if no such shader storage block exists or if it
	is not active.
	See [glfunc GetProgramResourceIndex].
	>*/
};

//]
//[oglplus_shader_storage_block_common_ops

template <>
class __ProgVarCommonOps<__tag_ShaderStorageBlock>
 : public __ProgVarLoc<__tag_ShaderStorageBlock>
{
public:
	static GLuint MaxIn(__ShaderType shader_type); /*<
	Returns the maximum number of shader storage blocks for a [^shader_type].
	>*/

	GLuint64 DataSize(void) const; /*<
	Returns the size of the shader storage block.
	See [glfunc GetActiveShaderStorageBlock], [glconst UNIFORM_BLOCK_DATA_SIZE].
	>*/

	void Binding(__ShaderStorageBufferBindingPoint binding); /*<
	Sets up the shader storage block binding.
	See [glfunc ShaderStorageBlockBinding].
	>*/
};

//]
//[oglplus_shader_storage_block_def

template <typename T>
using ShaderStorage = __ProgVar<
	__tag_ImplicitSel,
	__tag_ShaderStorageBlock,
	__tag_NoTypecheck,
	void
>; /*<
[^ShaderStorageBlock] indirectly inherits from __ProgVarLocOps_ShaderStorageBlock and __ProgVarCommonOps_ShaderStorage.
>*/

//]

