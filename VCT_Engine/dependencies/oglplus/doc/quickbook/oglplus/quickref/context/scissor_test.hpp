/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_ScissorTest
namespace context {

struct ScissorRectangle
{
	GLint X(void) const
	noexcept;

	GLint Y(void) const
	noexcept;

	GLint Left(void) const
	noexcept;

	GLint Bottom(void) const
	noexcept;

	GLint Width(void) const
	noexcept;

	GLint Height(void) const
	noexcept;
};

class ScissorTest
{
public:
	static void Scissor(
		GLint left,
		GLint bottom,
		__SizeType width,
		__SizeType height
	); /*<
	Defines the scissor rectangle for the first viewport.
	See [glfunc Scissor].
	>*/

#if GL_VERSION_4_1 || GL_ARB_viewport_array
	static void Scissor(
		__ViewportIndex viewport,
		GLint left,
		GLint bottom,
		__SizeType width,
		__SizeType height
	); /*<
	Defines the scissor rectangle for the specified [^viewport].
	See [glfunc ScissorIndexed].
	>*/
	static void Scissor(__ViewportIndex viewport, GLint* v);

	static void ScissorArray(GLuint first, __SizeType count, GLint* v); /*<
	Defines scissor rectangles for viewports specified
	by [^first] and [^count].
	See [glfunc ScissorArray].
	>*/

	static __context_ScissorRectangle ScissorBox(__ViewportIndex viewport); /*<
	Returns the extents of scissor box of the specified [^viewport].
	See [glfunc Get], [glconst SCISSOR_BOX].
	>*/
#endif
};

} // namespace context
//]

