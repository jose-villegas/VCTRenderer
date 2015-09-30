/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_ClientContext_example_1

ClientContext gl; /*<
Stateful (caching) wrapper for a GL context.
During the construction of this instance the values of current
GL context variables are queried and stored internally.
>*/


gl.ClearColor.Set(0.3f, 0.3f, 0.3f, 0.0f);
gl.ClearDepth.Set(1.0f);
gl.Caps.DepthTest.Disable();
gl.Caps.Blend.Enable();
gl.BlendFunc.Set(__BlendFunction::SrcAlpha, BlendFunction::OneMinusSrcAlpha);

gl.LineWidth.Set(1.5f);

gl.Viewport.Set(800, 600);

gl.Clear().ColorBuffer().DepthBuffer().StencilBuffer();

__Program prog;
__VertexArray vao;

/* ... */

gl.Program.Bind(prog); /*<
When using __ClientContext, then [^__Program::Use] must not be used and
programs must be made current this way, in order to keep the client context
in a consistent state.
>*/
gl.VertexArray.Bind(vao); /*<
Likewise, other objects bound to their binding points, must be bound
through the client context wrapper, not by [^Object::Bind].
>*/

{
	auto no_blend = gl.Caps.Blend.Push(false); /*<
	Push a new value [^false] (= ['disabled]) for [^__Capability::Blend].
	The previous setting (whatever it was) remains stored on the stack
	and GL is updated only if the new setting is different from the
	currently specified.
	Note that for this to work, the result of [^Push] [*must] be stored
	in a named variable.
	>*/
	auto no_depth = gl.Caps.DepthTest.Push(false);

	gl.DrawArrays(__PrimitiveType::TriangleStrip, 0, n);

	assert(no_depth); /*<
	The setting holders returned by [^Push], can be tested if they
	still hold the pushed value.
	>*/
	no_depth.Pop(); /*<
	Explicitly pop the pushed setting of [^Capability::DepthTest] and
	restored the previous one (actually call GL only if they differ).
	>*/
	assert(!no_depth); /*<
	After the value in the holder has been popped conversion to [^bool]
	returns [^false] and must not be popped again.
	>*/

	gl.DrawArrays(__PrimitiveType::TriangleStrip, 0, n);
} /*<
When the [^no_blend] instance goes out of scope and is destroyed, then
the setting is automatically popped from the stack.
>*/


gl.DrawElements(__PrimitiveType::TriangleStrip, m, __DataType::UnsignedInt);

//]

