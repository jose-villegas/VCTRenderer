/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_ViewportState
namespace client {

class ViewportState
{
	__SettingStackIndexed<
		__SettingStack<__context_ViewportExtents, ...>,
		__context_ViewportExtents,
		__ViewportIndex
	> Viewport; /*<
	Indexed set of stacks managing the extents
	for individual viewports.
	>*/

	__SettingStackIndexed<
		__SettingStack<__context_ViewportDepthRange, ...>,
		__context_ViewportDepthRange,
		__ViewportIndex
	> DepthRange; /*<
	Indexed set of stacks managing the depth ranges
	for individual viewports.
	>*/
};

} // namespace client
//]

