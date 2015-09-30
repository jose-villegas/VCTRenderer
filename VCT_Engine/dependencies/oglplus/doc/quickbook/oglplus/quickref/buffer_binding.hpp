/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_buffer_binding

#if GL_VERSION_4_0 || GL_ARB_transform_feedback3
class UniformBufferBindingPoint
 : public __LimitedCount<GL_MAX_UNIFORM_BUFFER_BINDINGS>
{
public:
	UniformBufferBindingPoint(GLuint number); /*<
	Throws __LimitError if [^number] is greater than
	the implementation-dependent maximum number of indexed
	uniform buffer binding points.
	See [glconst MAX_UNIFORM_BUFFER_BINDINGS].
	>*/
};
#endif

#if GL_VERSION_4_0 || GL_ARB_transform_feedback3
class TransformFeedbackBufferBindingPoint
 : public __LimitedCount<GL_MAX_TRANSFORM_FEEDBACK_BUFFERS>
{
public:
	TransformFeedbackBufferBindingPoint(GLuint number); /*<
	Throws __LimitError if [^number] is greater than
	the implementation-dependent maximum number of indexed
	transform feedback buffer binding points.
	See [glconst MAX_TRANSFORM_FEEDBACK_BUFFERS].
	>*/
};
#endif

#if GL_VERSION_4_2 || GL_ARB_shader_atomic_counters
class AtomicCounterBufferBindingPoint
 : public __LimitedCount<GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS>
{
public:
	AtomicCounterBufferBindingPoint(GLuint number); /*<
	Throws __LimitError if [^number] is greater than
	the implementation-dependent maximum number of indexed
	atomic counter buffer binding points.
	See [glconst MAX_ATOMIC_COUNTER_BUFFER_BINDINGS].
	>*/
};
#endif

#if GL_VERSION_4_3 || GL_ARB_shader_storage_buffer_object
class ShaderStorageBufferBindingPoint
 : public __LimitedCount<GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS>
{
public:
	ShaderStorageBufferBindingPoint(GLuint number); /*<
	Throws __LimitError if [^number] is greater than
	the implementation-dependent maximum number of indexed
	shader storage buffer binding points.
	See [glconst MAX_SHADER_STORAGE_BUFFER_BINDINGS].
	>*/
};
#endif

//]

