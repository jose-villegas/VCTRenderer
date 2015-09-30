/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_LogicOpState
namespace context {

class LogicOpState
{
public:
#if GL_VERSION_3_0
	static void LogicOp(__ColorLogicOperation op); /*<
	Sets the color logic operation.
	See [glfunc LogicOp].
	>*/

	static __ColorLogicOperation LogicOpMode(void); /*<
	Returns the current color logic operation.
	See [glfunc LogicOp], [glconst LOGIC_OP_MODE].
	>*/
#endif
};

} // namespace context
//]

