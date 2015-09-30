/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_ClipControlState
namespace client {

class ClipControlState
{
public:
	__SettingStack<__context_ClipControlParams, ...> ClipControl; /*<
	Stack managing the setting of clipping control parameters
	>*/
};

} // namespace client
//]

