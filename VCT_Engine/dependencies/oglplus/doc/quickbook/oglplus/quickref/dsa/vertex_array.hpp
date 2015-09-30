/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_dsa_vertex_array_1

template <>
class __ObjectOps<__tag_DirectState, __tag_VertexArray>
 : public __ObjZeroOps<__tag_DirectState, __tag_VertexArray> /*<
Indirectly inherits from __ObjCommonOps_VertexArray<__tag_VertexArray>.
>*/
{
public:
	ObjectOps& ElementBuffer(__BufferName buffer); /*<
	Bind buffer to [^this] VAO's element buffer binding point.
	See [glfunc VertexArrayElementBuffer].
	>*/

	ObjectOps& EnableVertexAttrib(__VertexAttribSlot location); /*<
	Disable the specified vertex attribute array.
	See [glfunc EnableVertexArrayAttrib].
	>*/

	ObjectOps& DisableVertexAttrib(__VertexAttribSlot location); /*<
	Enable the specified vertex attribute array.
	See [glfunc DisableVertexArrayAttrib].
	>*/
};
//]
//[oglplus_dsa_vertex_array_def

typedef __ObjectOps<__tag_DirectState, __tag_VertexArray>
	DSAVertexArrayOps;

typedef __Object<DSAVertexArrayOps> DSAVertexArray;

//]

