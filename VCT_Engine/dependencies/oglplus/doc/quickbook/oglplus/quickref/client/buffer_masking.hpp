/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_BufferMaskingState
namespace client {

class BufferMaskingState
{
public:
	__SettingStackIndexed<
		__SettingStack<__context_RGBAMask, ...>,
		__context_RGBAMask,
		__DrawBufferIndex
	> ColorMask; /*<
	Stack managing the setting of color channel mask.
	>*/

	__SettingStack<__Boolean, ...> DepthMask; /*<
	Stack managing the setting of depth mask.
	>*/

private:
	template <__SingleFace Value>
	class StencilMaskStack /*<
	This class template is here for documentation purposes only
	and is [*not] part of the public interface.
	>*/
	 : __SettingStack<__Boolean, ...>
	{ };
public:

	__enums_EnumToClass_SingleFace<
		__Nothing,
		__SingleFace,
		StencilMaskStack
	> StencilMask; /*<
	Stacks managing the setting of stencil mask separatelly for the front
	and the back face.
	>*/
};

} // namespace client
//]

