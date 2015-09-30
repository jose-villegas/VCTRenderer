/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_ScissorTestState
namespace client {

class ScissorTestState
{
	__SettingStackIndexed<
		__SettingStack<__context_ScissorRectangle, ...>,
		__context_ScissorRectangle,
		__ViewportIndex
	> Scissor; /*<
	Indexed set of stacks managing the scissor rectangles
	for the individual viewports.
	>*/
};

} // namespace client
//]

