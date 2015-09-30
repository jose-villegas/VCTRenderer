/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_BlendingOps
namespace client {

using oglplus::__context_BlendingOps;

} // namespace client
//]
//[oglplus_client_BlendingState
namespace client {

class BlendingState
{
public:
	__SettingStackIndexed<
		__SettingStack<__context_BlendEquationSeparate, ...>,
		__context_BlendEquationSeparate,
		__DrawBufferIndex
	> BlendEquation; /*<
	Indexed set of stacks managing the blend equations for the individual
	draw buffers.
	>*/

	__SettingStackIndexed<
		__SettingStack<__context_BlendFunctionSeparate, ...>,
		__context_BlendFunctionSeparate,
		__DrawBufferIndex
	> BlendEquation; /*<
	Indexed set of stacks managing the blend functions for the individual
	draw buffers.
	>*/

	__SettingStack<__context_RGBAValue, ...> BlendColor; /*<
	Stack managing the setting of blend color.
	>*/
};

} // namespace client
//]

