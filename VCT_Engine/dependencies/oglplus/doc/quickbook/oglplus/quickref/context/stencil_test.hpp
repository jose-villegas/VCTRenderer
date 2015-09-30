/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_StencilFuncArgs
namespace context {

struct StencilFuncArgs
{
	StencilFuncArgs(void)
	noexcept;

	StencilFuncArgs(
		__CompareFunction func,
		GLint refv = GLint(0),
		GLuint mask = ~GLuint(0)
	) noexcept;

	__CompareFunction Func(void) const
	noexcept;

	GLint Ref(void) const
	noexcept;

	GLuint ValueMask(void) const
	noexcept;

	friend
	bool operator == (const StencilFuncArgs& a, const StencilFuncArgs& b)
	noexcept;

	friend
	bool operator != (const StencilFuncArgs& a, const StencilFuncArgs& b)
	noexcept;
};

//]
//[oglplus_context_StencilOperations
struct StencilOperations
{
	StencilOperations(void)
	noexcept;

	StencilOperations(
		__StencilOperation sfail,
		__StencilOperation dfail,
		__StencilOperation dpass
	) noexcept;

	__StencilOperation StencilFail(void) const
	noexcept;

	__StencilOperation DepthFail(void) const
	noexcept;

	__StencilOperation DepthPass(void) const
	noexcept;

	friend
	bool operator == (const StencilOperations& a, const StencilOperations& b)
	noexcept;

	friend
	bool operator != (const StencilOperations& a, const StencilOperations& b)
	noexcept;
};

//]
//[oglplus_context_StencilTest

class StencilTest
{
public:
	static void StencilFunc(
		__CompareFunction function,
		GLint ref = GLint(0),
		GLuint mask = ~GLuint(0)
	); /*<
	Sets the stencil [^function], [^ref]erence value and a [^mask].
	See [glfunc StencilFunc].
	>*/
	static void StencilFunc(const StencilFuncArgs& fa);


	static void StencilFuncSeparate(
		__Face face,
		__CompareFunction function,
		GLint ref = GLint(0),
		GLuint mask = ~GLuint(0)
	); /*<
	Sets the stencil [^function] separately for front and back [^face].
	See [glfunc StencilFuncSeparate].
	>*/
	static void StencilFuncSeparate(
		__Face face,
		const __context_StencilFuncArgs& fa
	);
	static void StencilFuncSeparateSingle(
		__SingleFace face,
		__CompareFunction function,
		GLint ref = GLint(0),
		GLuint mask = ~GLuint(0)
	);
	static void StencilFuncSeparateSingle(
		__SingleFace face,
		const __context_StencilFuncArgs& fa
	);

	static void StencilOp(
		__StencilOperation sfail,
		__StencilOperation dfail,
		__StencilOperation dpass
	); /*<
	Sets the stencil operations.
	See [glfunc StencilOp].
	>*/

	static void StencilOpSeparate(
		__Face face,
		__StencilOperation sfail,
		__StencilOperation dfail,
		__StencilOperation dpass
	); /*<
	Sets the stencil operations separately for front and back [^face].
	See [glfunc StencilOpSeparate].
	>*/
	static void StencilOpSeparateSingle(
		__SingleFace face,
		__StencilOperation sfail,
		__StencilOperation dfail,
		__StencilOperation dpass
	);

	static __CompareFunction StencilFunc(bool backface = false); /*<
	Returns the currently set stencil function.
	See [glfunc Get], [glconst STENCIL_FUNC], [glconst STENCIL_BACK_FUNC].
	>*/
	static CompareFunction StencilFunc(__Face face);

	static GLuint StencilValueMask(bool backface = false); /*<
	Returns the currently set value of stencil mask.
	See [glfunc Get], [glconst STENCIL_VALUE_MASK], [glconst STENCIL_BACK_VALUE_MASK].
	>*/
	static GLuint StencilValueMask(__Face face);

	static GLuint StencilRef(bool backface = false); /*<
	Returns the currently set stencil reference value.
	See [glfunc Get], [glconst STENCIL_REF], [glconst STENCIL_BACK_REF].
	>*/
	static GLuint StencilRef(__Face face);

	static __context_StencilFuncArgs StencilFuncArgs(__Face face); /*<
	Returns the currently set stencil function arguments.
	>*/
	static __context_StencilFuncArgs StencilFuncArgsSingle(__SingleFace face);

	static __StencilOperation StencilFail(bool backface = false); /*<
	Returns the stencil-fail action.
	See [glfunc Get], [glconst STENCIL_FAIL], [glconst STENCIL_BACK_FAIL].
	>*/
	static StencilOperation StencilFail(__Face face);

	static __StencilOperation StencilPassDepthFail(bool backface = false); /*<
	Returns the stencil-pass depth-fail action.
	See [glfunc Get], [glconst STENCIL_PASS_DEPTH_FAIL], [glconst STENCIL_BACK_PASS_DEPTH_FAIL].
	>*/
	static StencilOperation StencilPassDepthFail(__Face face);

	static __StencilOperation StencilPassDepthPass(bool backface = false); /*<
	Returns the stencil-pass depth-fail action.
	See [glfunc Get], [glconst STENCIL_PASS_DEPTH_PASS], [glconst STENCIL_BACK_PASS_DEPTH_PASS].
	>*/
	static StencilOperation StencilPassDepthPass(__Face face)

	static __context_StencilOperations StencilOps(bool backface = false);
	static StencilOperations StencilOps(__Face face);
	static StencilOperations StencilOpsSingle(__SingleFace face);
};

} // namespace context
//]

