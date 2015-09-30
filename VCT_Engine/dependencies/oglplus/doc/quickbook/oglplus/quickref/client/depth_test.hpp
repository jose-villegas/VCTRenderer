/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_DepthTestState
namespace client {

class DepthTestState
{
public:
	__SettingStack<__CompareFunction, ...> DepthFunc; /*<
	Stack managing the setting of depth compare function.
	>*/
};

} // namespace client
//]

