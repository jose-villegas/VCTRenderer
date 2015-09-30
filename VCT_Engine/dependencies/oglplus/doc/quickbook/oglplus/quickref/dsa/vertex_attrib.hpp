/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_dsa_vertex_array_attrib

#if GL_VERSION_4_5 || GL_ARB_direct_state_access

class DSAVertexArrayAttrib
 : public __ProgVarCommonOps_VertexAttrib<__tag_VertexAttrib>
{
public:
	DSAVertexArrayAttrib(
		__VertexArrayName vao,
		__VertexAttribSlot location
	); /*<
	See [glfunc GetAttribLocation].
	>*/

	DSAVertexArrayAttrib(
		__VertexArrayName vao,
		__ProgramName program,
		__StrCRef identifier
	); /*<
	See [glfunc GetAttribLocation].
	>*/

	DSAVertexArrayAttrib& Enable(void); /*<
	Enables [^this] vertex array attribute.
	See [glfunc EnableVertexArrayAttrib].
	>*/

	DSAVertexArrayAttrib& Disable(void); /*<
	Disables [^this] specified vertex array attribute
	See [glfunc DisableVertexArrayAttrib].
	>*/

	DSAVertexArrayAttrib& VertexBuffer(
		__BufferName buffer,
		GLintptr offset,
		__SizeType stride
	); /*<
	Sets the vertex buffer for [^this] vertex array attribute.
	See [glfunc VertexArrayVertexBuffer].
	>*/

	DSAVertexArrayAttrib& Format(
		GLint values_per_vertex,
		__DataType data_type,
		__Boolean normalized,
		GLuint relative_offset
	); /*<
	Sets up the properties of [^this] vertex array attribute.
	See [glfunc VertexArrayAttribFormat].
	>*/

	DSAVertexArrayAttrib& IFormat(
		GLint values_per_vertex,
		__DataType data_type,
		GLuint relative_offset
	); /*<
	Sets up the properties of [^this] vertex array attribute.
	See [glfunc VertexArrayAttribIFormat].
	>*/

	DSAVertexArrayAttrib& LFormat(
		GLint values_per_vertex,
		__DataType data_type,
		GLuint relative_offset
	); /*<
	Sets up the properties of [^this] vertex array attribute.
	See [glfunc VertexArrayAttribLFormat].
	>*/
};

#endif

//]

