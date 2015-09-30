/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_DrawingState
namespace context {

class DrawingState
{
public:
#if GL_VERSION_3_1
	static void PrimitiveRestartIndex(GLuint index); /*<
	Sets the primitive restart index.
	See [glfunc PrimitiveRestartIndex].
	>*/
	static GLuint PrimitiveRestartIndex(void);
#endif

#if GL_ARB_tessellation_shader || GL_VERSION_4_0
	static void PatchParameter(
		__PatchParameter parameter,
		GLint value
	); /*<
	Sets a patch parameter value.
	See [glfunc PatchParameter].
	>*/
#endif
};
//]
//[oglplus_context_DrawingOps1

class DrawingOps
{
public:
	static void DrawArrays(
		__PrimitiveType primitive,
		GLint first,
		__SizeType count
	); /*<
	Draws a range of primitives of the specified type from
	the bound array buffers.
	See [glfunc DrawArrays].
	>*/

#if GL_VERSION_3_1
	static void DrawArraysInstanced(
		__PrimitiveType primitive,
		GLint first,
		__SizeType count,
		__SizeType inst_count
	); /*<
	Draws multiple instances of a range of elements.
	See [glfunc DrawArraysInstanced].
	>*/
#endif

#if GL_VERSION_4_2
	static void DrawArraysInstancedBaseInstance(
		__PrimitiveType primitive,
		GLint first,
		__SizeType count,
		__SizeType inst_count,
		__SizeType base_instance
	); /*<
	Draws multiple instances of a range of elements with
	offset applied to instanced attributes.
	See [glfunc DrawArraysInstancedBaseInstance].
	>*/
#endif

#if GL_VERSION_4_0 || GL_ARB_draw_indirect
	static void DrawArraysIndirect(
		__PrimitiveType primitive,
		const void* indirect = nullptr
	); /*<
	Draws primitives from an indirect buffer.
	See [glfunc DrawArraysIndirect].
	>*/
#endif

#if GL_VERSION_3_0
	static void MultiDrawArrays(
		__PrimitiveType primitive,
		const GLint* first,
		const GLsizei* count,
		__SizeType primcount
	); /*<
	Draws multiple primcount ranges of primitives from the bound
	array buffers.
	See [glfunc MultiDrawArrays].
	>*/
#endif

#if GL_VERSION_4_3 || GL_ARB_multi_draw_indirect
	static void MultiDrawArraysIndirect(
		__PrimitiveType primitive,
		__SizeType draw_count,
		__SizeType stride = 0,
		const void* indirect = nullptr
	); /*<
	See [glfunc MultiDrawArraysIndirect].
	>*/
#endif

	static void DrawElements(
		__PrimitiveType primitive,
		__SizeType count,
		__DataType data_type
	); /*<
	See [glfunc DrawElements].
	>*/

	template <typename T>
	static
	void DrawElements(
		__PrimitiveType primitive,
		__SizeType count,
		const T* indices
	);

	static void DrawElementsInstanced(
		__PrimitiveType primitive,
		__SizeType count,
		__DataType data_type,
		__SizeType instance_count
	); /*<
	See [glfunc DrawElementsInstanced].
	>*/

//]
//[oglplus_context_DrawingOps2
	template <typename T>
	static
	void DrawElementsInstanced(
		__PrimitiveType primitive,
		__SizeType count,
		const T* indices,
		__SizeType instance_count
	);

#if GL_VERSION_4_2
	static void DrawElementsInstancedBaseInstance(
		__PrimitiveType primitive,
		__SizeType count,
		__DataType data_type,
		__SizeType inst_count,
		GLuint base_instance
	); /*<
	See [glfunc DrawElementsInstancedBaseInstance].
	>*/

	template <typename T>
	static
	void DrawElementsInstancedBaseInstance(
		__PrimitiveType primitive,
		__SizeType count,
		const T* indices,
		__SizeType inst_count,
		GLuint base_instance
	);
#endif

	static void MultiDrawElements(
		__PrimitiveType primitive,
		const GLsizei* count,
		__DataType data_type,
		__SizeType draw_count
	); /*<
	See [glfunc MultiDrawElements].
	>*/

	template <typename T>
	static
	void MultiDrawElements(
		__PrimitiveType primitive,
		const GLsizei* count,
		T* const * indices,
		__SizeType draw_count
	);

	static void DrawRangeElements(
		__PrimitiveType primitive,
		GLuint start,
		GLuint end,
		__SizeType count,
		__DataType data_type
	);  /*<
	See [glfunc DrawRangeElements].
	>*/

	template <typename T>
	static
	void DrawRangeElements(
		__PrimitiveType primitive,
		GLuint start,
		GLuint end,
		__SizeType count,
		const T* indices
	);

#if GL_VERSION_4_0 || GL_ARB_draw_indirect
	static void DrawElementsIndirect(
		PrimitiveType primitive,
		DataType data_type,
		const void* indirect = nullptr
	); /*<
	See [glfunc DrawElementsIndirect].
	>*/
#endif

#if GL_VERSION_4_3
	static void MultiDrawElementsIndirect(
		__PrimitiveType primitive,
		__DataType data_type,
		__SizeType draw_count,
		__SizeType stride = 0,
		const void* indirect = nullptr
	); /*<
	See [glfunc MultiDrawElementsIndirect].
	>*/
#endif

#if GL_VERSION_3_2 || GL_ARB_draw_elements_base_vertex
	static void DrawElementsBaseVertex(
		__PrimitiveType primitive,
		__SizeType count,
		__DataType data_type,
		GLint base_vertex
	); /*<
	See [glfunc DrawElementsBaseVertex].
	>*/

	template <typename T>
	static
	void DrawElementsBaseVertex(
		__PrimitiveType primitive,
		__SizeType count,
		const T* indices,
		GLint base_vertex
	);

	static void DrawRangeElementsBaseVertex(
		__PrimitiveType primitive,
		GLuint start,
		GLuint end,
		__SizeType count,
		__DataType data_type,
		GLint base_vertex
	); /*<
	See [glfunc DrawRangeElementsBaseVertex].
	>*/

	template <typename T>
	static
	void DrawRangeElementsBaseVertex(
		__PrimitiveType primitive,
		GLuint start,
		GLuint end,
		__SizeType count,
		const T* indices,
		GLint base_vertex
	);

	static void DrawElementsInstancedBaseVertex(
		__PrimitiveType primitive,
		__SizeType count,
		__DataType data_type,
		__SizeType inst_count,
		GLint base_vertex
	); /*<
	See [glfunc DrawElementsInstancedBaseVertex].
	>*/

	template <typename T>
	static
	void DrawElementsInstancedBaseVertex(
		__PrimitiveType primitive,
		__SizeType count,
		const T* indices,
		__SizeType inst_count,
		GLint base_vertex
	);

	static void MultiDrawElementsBaseVertex(
		__PrimitiveType primitive,
		const GLsizei* count,
		__DataType data_type,
		__SizeType draw_count,
		const GLint* base_vertex
	); /*<
	See [glfunc MultiDrawElementsBaseVertex].
	>*/

	template <typename T>
	static
	void MultiDrawElementsBaseVertex(
		__PrimitiveType primitive,
		const GLsizei* count,
		T* const * indices,
		__SizeType draw_count,
		const GLint* base_vertex
	);
#endif

#if GL_VERSION_4_2
	static void DrawElementsInstancedBaseVertexBaseInstance(
		__PrimitiveType primitive,
		__SizeType count,
		__DataType data_type,
		__SizeType inst_count,
		GLint base_vertex,
		GLuint base_instance
	); /*<
	See [glfunc DrawElementsInstancedBaseVertexBaseInstance].
	>*/

	template <typename T>
	static
	void DrawElementsInstancedBaseVertexBaseInstance(
		__PrimitiveType primitive,
		__SizeType count,
		const T* indices,
		__SizeType inst_count,
		GLint base_vertex,
		GLuint base_instance
	);
#endif

#if GL_VERSION_4_0 || GL_ARB_transform_feedback2
	static void DrawTransformFeedback(
		__PrimitiveType primitive,
		__TransformFeedbackName xfb
	); /*<
	See [glfunc DrawTransformFeedback].
	>*/
#endif

#if GL_NV_draw_texture
	static void DrawTexture(
		__TextureName texture,
		__SamplerName sampler,
		GLfloat x0,
		GLfloat y0,
		GLfloat x1,
		GLfloat y1,
		GLfloat z,
		GLfloat s0,
		GLfloat t0,
		GLfloat s1,
		GLfloat t1
	); /*<
	See [glfunc DrawTextureNV].
	>*/
#endif
};

} // namespace context
//]

