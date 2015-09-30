/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_dsa_transform_feedback_1

template <>
class __ObjectOps<__tag_DirectState, __tag_TransformFeedback>
 : public __ObjZeroOps<__tag_DirectState, __tag_TransformFeedback>
{
public:
	__Boolean Active(void) const; /*<
	Returns true if transform feedback is active.
	See [glfunc GetNamedTransformFeedback],
	[glconst GL_TRANSFORM_FEEDBACK_ACTIVE]
	>*/

	__Boolean Paused(void) const; /*<
	Returns true if transform feedback is paused.
	See [glfunc GetNamedTransformFeedback],
	[glconst GL_TRANSFORM_FEEDBACK_PAUSED]
	>*/

	ObjectOps& BufferBase(GLuint index, __BufferName buffer); /*<
	See [glfunc TransformFeedbackBufferBase].
	>*/

	ObjectOps& BufferRange(
		GLuint index,
		__BufferName buffer,
		__BufferSize offset,
		__BufferSize size
	); /*<
	See [glfunc TransformFeedbackBufferRange].
	>*/

	__BufferName BufferBinding(GLuint index) const; /*<
	See [glfunc GetNamedTransformFeedback],
	[glconst GL_TRANSFORM_FEEDBACK_BUFFER_BINDING].
	>*/

	GLint64 BufferStart(GLuint index) const; /*<
	See [glfunc GetNamedTransformFeedback],
	[glconst GL_TRANSFORM_FEEDBACK_BUFFER_START].
	>*/

	GLint64 BufferSize(GLuint index) const; /*<
	See [glfunc GetNamedTransformFeedback],
	[glconst GL_TRANSFORM_FEEDBACK_BUFFER_SIZE].
	>*/
};

//]
//[oglplus_dsa_transform_feedback_def

#if GL_VERSION_4_0 || GL_ARB_transform_feedback2
typedef ObjectOps<__tag_ImplicitSel, __tag_TransformFeedback>
	TransformFeedbackOps;

typedef __Object<TransformFeedbackOps> TransformFeedback;
#endif

typedef __ObjectZero<__ObjZeroOps<__tag_ImplicitSel, __tag_TransformFeedback>>
	DefaultTransformFeedback;

//]

