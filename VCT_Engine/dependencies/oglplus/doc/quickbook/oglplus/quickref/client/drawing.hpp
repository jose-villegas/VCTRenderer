/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_DrawingOps
namespace client {

using oglplus::__context_DrawingOps;

} // namespace client
//]
//[oglplus_client_DrawingState
namespace client {

class DrawingState
{
public:
	__SettingStack<GLint, ...> PrimitiveRestartIndex; /*<
	Stack managing the setting of primitive restart index.
	>*/
};

} // namespace client
//]

