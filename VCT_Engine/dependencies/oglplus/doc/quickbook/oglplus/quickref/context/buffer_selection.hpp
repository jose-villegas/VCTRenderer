/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_BufferSelection
namespace context {

class BufferSelection
{
public:
	typedef __OneOf<
		oglplus::__ColorBuffer,
		oglplus::__FramebufferColorAttachment
	> ColorBuffer; /*<
	Color buffer specification type.
	>*/

	static void DrawBuffer(ColorBuffer buffer); /*<
	Sets the destination color buffer for draw operations.
	See [glfunc DrawBuffer].
	>*/
	static void DrawBuffers(const __EnumArray<ColorBuffer>& buffers); /*<
	Sets the destination color buffers for draw operations.
	See [glfunc DrawBuffers].
	>*/
	static void DrawBuffers(__SizeType count, const ColorBuffer* buffers);

	static void ReadBuffer(ColorBuffer buffer); /*<
	Sets the source color buffer for read operations.
	See [glfunc ReadBuffer].
	>*/
};

} // namespace context
//]

