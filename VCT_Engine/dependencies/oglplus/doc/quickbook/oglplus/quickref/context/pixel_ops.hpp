/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_PixelState
namespace context {

class PixelState
{
public:
	static void PixelStore(__PixelParameter parameter, GLfloat value); /*<
	Sets the [^value] of a pixel storage [^parameter].
	See [glfunc PixelStore].
	>*/
	static void PixelStore(__PixelParameter parameter, GLint value);

	template <__PixelParameter Parameter>
	static void PixelStore(__Unspecified value); /*<
	Sets the [^value] of a pixel storage [^Parameter].
	[^value] has an appropriate type to hold the value
	of the [^Parameter] being set.
	See [glfunc PixelStore].
	>*/

	static GLfloat PixelStoreValue(
		__PixelParameter parameter,
		__TypeTag<float>
	); /*<
	Gets the [^value] of a pixel storage [^Parameter].
	See [glfunc Get].
	>*/
	static GLint PixelStoreValue(
		__PixelParameter parameter,
		__TypeTag<int>
	);
	static __Boolean PixelStoreValue(
		__PixelParameter parameter,
		__TypeTag<bool>
	);

	template <__PixelParameter Parameter>
	static __Unspecified PixelStoreValue(void); /*<
	Gets the [^value] of a pixel storage [^Parameter].
	The return value has an appropriate type to hold the value
	of the [^Parameter] being retrieved.
	See [glfunc Get].
	>*/
};

//]
//[oglplus_context_PixelOps
class PixelOps
{
public:

	static void ReadPixels(
		GLint x,
		GLint y,
		__SizeType width,
		__SizeType height,
		__PixelDataFormat format,
		__PixelDataType type,
		void* data
	); /*<
	Reads a block of pixels from the current framebuffer.
	See [glfunc ReadPixels].
	>*/

	static void BlitFramebuffer(
		GLint srcX0,
		GLint srcY0,
		GLint srcX1,
		GLint srcY1,
		GLint dstX0,
		GLint dstY0,
		GLint dstX1,
		GLint dstY1,
		__Bitfield<__BufferSelectBit> mask,
		__BlitFilter filter
	); /*<
	Transfers a rectangle of pixels from the read buffer the draw buffer.
	See [glfunc blitFramebuffer].
	>*/
};

} // namespace context
//]

