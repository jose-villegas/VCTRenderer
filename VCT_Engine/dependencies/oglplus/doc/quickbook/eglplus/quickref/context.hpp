/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[eglplus_context_attribs

typedef __AttributeList<__ContextAttrib, ...> ContextAttribs; /*<
Attribute list type for context attributes.
>*/

typedef __FinishedAttributeList<__ContextAttrib, ...> FinishedContextAttribs; /*<
Finished attribute list type for context attributes.
>*/

//]
//[eglplus_context_1

class Context
{
public:
	Context(Context&& tmp);
	Context(const Context& tmp) = delete;

	Context(
		const __Display& display,
		const __Config& config
	); /*<
	Constructs a non-sharing context without any attributes.
	See [eglfunc CreateContext].
	>*/

	Context(
		const __Display& display,
		const __Config& config,
		const Context& shared_context
	); /*<
	Construct a sharing context without any attributes.
	See [eglfunc CreateContext].
	>*/

	Context(
		const __Display& display,
		const __Config& config,
		const __FinishedContextAttribs& attribs
	); /*<
	Construct a non-sharing context with the specified attributes.
	See [eglfunc CreateContext].
	>*/

	Context(
		const __Display& display,
		const __Config& config,
		const Context& shared_context,
		const __FinishedContextAttribs& attribs
	); /*<
	Construct a sharing context without any attributes.
	See [eglfunc CreateContext].
	>*/

	~Context(void); /*<
	Destroys the wrapped context.
	See [eglfunc DestroyContext].
	>*/

	__Boolean MakeCurrent(
		const __Surface& draw_surface,
		const __Surface& read_surface
	); /*<
	Makes the context current.
	See [eglfunc MakeCurrent].
	>*/
	__Boolean MakeCurrent(const __Surface& surface);

	__Boolean MakeCurrent(void); /*<
	Makes the context current without using any surfaces.
	See [eglfunc MakeCurrent], [eglconst NO_SURFACE].
	>*/

	__Boolean Release(void); /*<
	Releases the current context without assigning a new one.
	See [eglfunc MakeCurrent], [eglconst NO_SURFACE], [eglconst NO_CONTEXT].
	>*/
//]
//[eglplus_context_2

	__Boolean Query(__ContextAttrib attrib, EGLint& value) const; /*<
	Queries a context attribute.
	See [eglfunc QueryContext].
	>*/

	EGLint ConfigId(void) const; /*<
	Queries a context attribute.
	See [eglfunc QueryContext], [eglconst CONFIG_ID].
	>*/

	__Boolean WaitClient(void) const; /*<
	Waits for client API commands to complete.
	See [eglfunc WaitClient].
	>*/

	__Boolean WaitGL(void) const; /*<
	Waits for GL API commands to complete.
	See [eglfunc WaitGL].
	>*/

	__Boolean WaitNative(EGLint engine) const; /*<
	Waits for native API commands to complete.
	See [eglfunc WaitNative].
	>*/
};

::EGLContext GetEGLHandle(const __Context& context)
noexcept; /*<
Returns the EGL context handle wrapped by a __Context.
>*/

};

//]

