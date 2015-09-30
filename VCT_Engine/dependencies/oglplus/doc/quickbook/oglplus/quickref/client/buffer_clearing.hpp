/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_BufferClearingOps
namespace client {

using oglplus::__context_BufferClearingOps;

} // namespace client
//]
//[oglplus_client_BufferClearingState
namespace client {

class BufferClearingState
{
public:
	__SettingStack<__context_RGBAValue, ...> ClearColor; /*<
	Stack managing the setting of clear color.
	>*/

	__SettingStack<GLfloat, ...> ClearDepth; /*<
	Stack managing the setting of clear depth value.
	>*/

	__SettingStack<GLint, ...> ClearStencil; /*<
	Stack managing the setting of clear stencil value.
	>*/
};

} // namespace client
//]

