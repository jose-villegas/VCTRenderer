/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_BufferMaskingState
namespace context {

class BufferMaskingState
{
public:
	static void ColorMask(
		__Boolean r,
		__Boolean g,
		__Boolean b,
		__Boolean a
	); /*<
	Sets the color mask for the default color buffer.
	See [glfunc ColorMask].
	>*/
	static void ColorMask(const __context_RGBAMask&);
	static __context_RGBAMask ColorWriteMask(void); /*<
	Returns the value of color buffer write mask.
	See [glfunc Get], [glconst COLOR_WRITEMASK].
	>*/

#if GL_VERSION_3_0
	static void ColorMask(
		__DrawBufferIndex buffer,
		__Boolean r,
		__Boolean g,
		__Boolean b,
		__Boolean a
	); /*<
	Sets the color mask for the specified color [^buffer].
	See [glfunc ColorMaski].
	>*/
	static void ColorMask(__DrawBufferIndex buffer, const __context_RGBAMask&);
	static __context_RGBAMask ColorWriteMask(__DrawBufferIndex buffer); /*<
	Returns the value of color buffer write mask.
	See [glfunc Get], [glconst COLOR_WRITEMASK].
	>*/
#endif

	static void DepthMask(__Boolean mask); /*<
	Sets the depth buffer [^mask].
	See [glfunc DepthMask].
	>*/
	static __Boolean DepthWriteMask(void); /*<
	Returns the value of depth buffer write mask.
	See [glfunc Get], [glconst DEPTH_WRITEMASK].
	>*/

	static void StencilMask(GLuint mask); /*<
	Sets the stencil buffer [^mask];
	See [glfunc StencilMask].
	>*/
	static void StencilMaskSeparate(__Face face, GLuint mask); /*<
	Sets the stencil mask separately for front and back faces.
	See [glfunc StencilMaskSeparate].
	>*/
	static void StencilMaskSeparateSingle(__SingleFace face, GLuint mask);

	static GLuint StencilWriteMask(bool backface = false); /*<
	Returns the value of stencil buffer write mask.
	See [glfunc Get], [glconst STENCIL_WRITEMASK] and
	[glconst STENCIL_BACK_WRITEMASK].
	>*/
	static GLuint StencilWriteMask(__Face face);
	static GLuint StencilWriteMaskSingle(__SingleFace face);
};

} // namespace context
//]

