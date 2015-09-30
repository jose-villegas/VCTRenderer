/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_transform_feedback_common

template <>
class __ObjCommonOps<__tag_TransformFeedback>
 : public __TransformFeedbackName
{
public:
#if GL_VERSION_4_0 || GL_ARB_transform_feedback2
	typedef __TransformFeedbackTarget Target; /*<
	Transform feedback bind target.
	>*/

	static __TransformFeedbackName Binding(__TransformFeedbackTarget target); /*<
	Returns the transform feedback object currently bound
	to the specified [^target].
	See [glfunc GetIntegerv].
	>*/
	static void Bind(
		__TransformFeedbackTarget target,
		__TransformFeedbackName transform_feedback
	); /*<
	Binds the specified [^transform_feedback] to the specified [^target].
	See [glfunc BindTransformFeedback].
	>*/

	void Bind(__TransformFeedbackTarget target) const; /*<
	Binds [^this] transform feedback to the specified [^target].
	See [glfunc BindTransformFeedback].
	>*/
#endif
	static void Begin(__TransformFeedbackPrimitiveType mode); /*<
	Begins transform feedback in the specified mode.
	See [glfunc BeginTransformFeedback].
	[note Consider using an instance of [^Activator] class for more robustness.]
	>*/
	static void BeginPoints(void); /*<
	See [glconst POINTS].
	>*/
	static void BeginLines(void); /*<
	See [glconst LINES].
	>*/
	static void BeginTriangles(void); /*<
	See [glconst TRIANGLES].
	>*/

	static void End(void); /*<
	Ends the currently active transform feedback.
	See [glfunc EndTransformFeedback].
	>*/

	static void Pause(void); /*<
	Pauses currently active transform feedback.
	See [glfunc PauseTransformFeedback].
	[note Consider using an instance of [^Pauser] class for more robustness.]
	>*/

	static void Resume(void); /*<
	Resumes currently paused transform feedback.
	See [glfunc ResumeTransformFeedback].
	>*/

	typedef __TransformFeedbackActivator Activator;
	typedef __TransformFeedbackPauser Pauser;
};
//]
//[oglplus_transform_feedback_def

#if GL_VERSION_4_0 || GL_ARB_transform_feedback2
typedef ObjectOps<__tag_ImplicitSel, __tag_TransformFeedback> /*<
Indirectly inherits from __ObjCommonOps_TransformFeedback<__tag_TransformFeedback>
>*/
	TransformFeedbackOps;

typedef __Object<TransformFeedbackOps> TransformFeedback;
#endif

typedef __ObjectZero<__ObjZeroOps<__tag_ImplicitSel, __tag_TransformFeedback>> /*<
Indirectly inherits from __ObjCommonOps_TransformFeedback<__tag_TransformFeedback>
>*/
	DefaultTransformFeedback;

//]
//[oglplus_transform_feedback_activator
class TransformFeedbackActivator
{
public:
	TransformFeedbackActivator(const TransformFeedbackActivator&) = delete;
	TransformFeedbackActivator(TransformFeedbackActivator&& tmp);

	TransformFeedbackActivator(__TransformFeedbackPrimitiveType mode); /*<
	Begins transform feedback in the specified [^mode].
	See [glfunc BeginTransformFeedback].
	>*/

	~TransformFeedbackActivator(void) /*<
	Ends currently active transform feedback.
	See [glfunc EndTransformFeedback].
	>*/
	noexcept;

	void Finish(void); /*<
	Explicitly ends the currently active transform feedback.
	See [glfunc EndTransformFeedback].
	>*/
};
//]
//[oglplus_transform_feedback_pauser
class TransformFeedbackPauser
{
public:
	TransformFeedbackPauser(const TransformFeedbackPauser&) = delete;
	TransformFeedbackPauser(TransformFeedbackPauser&& tmp);

	TransformFeedbackPauser(void); /*<
	Pauses the currenly active transform feedback.
	See [glfunc PauseTransformFeedback].
	>*/

	~TransformFeedbackPauser(void) /*<
	Resumes the currently paused transform feedback.
	See [glfunc ResumeTransformFeedback].
	>*/
	noexcept;

	void Resume(void); /*<
	Explicitly resumes the currently paused transform feedback.
	See [glfunc ResumeTransformFeedback].
	>*/
};
//]

