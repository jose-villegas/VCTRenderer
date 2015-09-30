/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_Computing
namespace context {

class ComputingOps
{
public:
#if GL_VERSION_4_3
	static void DispatchCompute(
		GLuint num_groups_x,
		GLuint num_groups_y,
		GLuint num_groups_z
	); /*<
	Launches the specified number of compute work groups.
	See [glfunc DispatchCompute].
	>*/

	static void DispatchComputeIndirect(GLintptr indirect); /*<
	Launches indirectly several compute work groups.
	See [glfunc DispatchComputeIndirect].
	>*/
#endif
};

} // namespace context
//]

