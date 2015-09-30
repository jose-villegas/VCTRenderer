/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_Blending_helpers
namespace context {

struct BlendEquationSeparate
{
	BlendEquationSeparate(void)
	noexcept;

	BlendEquationSeparate(__BlendEquation rgb, __BlendEquation alpha)
	noexcept;

	__BlendEquation RGB(void) const
	noexcept;

	__BlendEquation Alpha(void) const
	noexcept;
};

struct BlendFunctionSeparate
{
	BlendFunctionSeparate(void)
	noexcept;

	BlendFunctionSeparate(
		__BlendFunction src_rgb,
		__BlendFunction src_alpha,
		__BlendFunction dst_rgb,
		__BlendFunction dst_alpha
	) noexcept;

	__BlendFunction SrcRGB(void) const
	noexcept;

	__BlendFunction SrcAlpha(void) const
	noexcept;

	__BlendFunction DstRGB(void) const
	noexcept;

	__BlendFunction DstAlpha(void) const
	noexcept;
};
//]
//[oglplus_context_BlendingOps
class BlendingOps
{
public:
#if GL_KHR_blend_equation_advanced
	static void BlendBarrier(void); /*<
	Specifies boundaries between blending passes.
	>*/
#endif
};
//]
//[oglplus_context_BlendingState
class BlendingState
{
public:
	static void BlendEquation(__OneOf<__BlendEquation, __BlendEquationAdvanced> eq); /*<
	Sets the blending equation.
	See [glfunc BlendEquation].
	>*/
	static void BlendEquationSeparate(
		__BlendEquation eq_rgb,
		__BlendEquation eq_alpha
	); /*<
	Sets the blend equation separately for RGB and alpha.
	See [glfunc BlendEquationSeparate].
	>*/

	static void BlendEquationSeparate(const __context_BlendEquationSeparate&);
	static __context_BlendEquationSeparate BlendEquationSeparate(void);

	static __BlendEquation BlendEquationRGB(void);
	static __BlendEquation BlendEquationAlpha(void);

#if GL_VERSION_4_0
	static void BlendEquation(
		__DrawBufferIndex buffer,
		__OneOf<__BlendEquation, __BlendEquationAdvanced> eq
	); /*<
	Sets the blend equation for a particular draw [^buffer]
	See [glfunc BlendEquationi].
	>*/
	static void BlendEquationSeparate(
		__DrawBufferIndex buffer,
		__BlendEquation eq_rgb,
		__BlendEquation eq_alpha
	); /*<
	Sets the blend equation separate for RGB and alpha for the specified
	draw [^buffer].
	See [glfunc BlendEquationSeparatei].
	>*/

	static void BlendEquationSeparate(
		__DrawBufferIndex buffer,
		const __context_BlendEquationSeparate&
	);
	static __context_BlendEquationSeparate BlendEquationSeparate(__DrawBufferIndex buffer);

	static __BlendEquation BlendEquationRGB(__DrawBufferIndex buffer);
	static __BlendEquation BlendEquationAlpha(__DrawBufferIndex buffer);
#endif

	static void BlendFunc(__BlendFunction src, __BlendFunction dst); /*<
	Sets the blend function. See [glfunc BlendFunc].
	>*/
	static void BlendFuncSeparate(
		__BlendFunction src_rgb,
		__BlendFunction dst_rgb,
		__BlendFunction src_alpha,
		__BlendFunction dst_alpha
	); /*<
	Sets the blend function separately for RGB and alpha.
	See [glfunc BlendFuncSeparate].
	>*/

	static void BlendFuncSeparate(const __context_BlendFunctionSeparate&);
	static __context_BlendFunctionSeparate BlendFuncSeparate(void);

	static __BlendFunction BlendFuncSrcRGB(void);
	static __BlendFunction BlendFuncSrcAlpha(void);
	static __BlendFunction BlendFuncDstRGB(void);
	static __BlendFunction BlendFuncDstAlpha(void);

#if GL_VERSION_4_0
	static void BlendFunc(
		__DrawBufferIndex buffer,
		__BlendFunction src,
		__BlendFunction dst
	); /*<
	Sets the blend function for a particular draw [^buffer].
	See [glfunc BlendFunci].
	>*/
	static void BlendFuncSeparate(
		__DrawBufferIndex buffer,
		__BlendFunction src_rgb,
		__BlendFunction dst_rgb,
		__BlendFunction src_alpha,
		__BlendFunction dst_alpha
	); /*<
	Sets the blend function separately for RGB and alpha for
	a particular draw [^buffer].
	See [glfunc BlendFuncSeparatei].
	>*/

	static void BlendFuncSeparate(
		__DrawBufferIndex buffer,
		const __context_BlendFunctionSeparate&
	);
	static __context_BlendFunctionSeparate BlendFuncSeparate(__DrawBufferIndex buffer);

	static __BlendFunction BlendFuncSrcRGB(__DrawBufferIndex buffer);
	static __BlendFunction BlendFuncSrcAlpha(__DrawBufferIndex buffer);
	static __BlendFunction BlendFuncDstRGB(__DrawBufferIndex buffer);
	static __BlendFunction BlendFuncDstAlpha(__DrawBufferIndex buffer);
#endif

	static void BlendColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a);

	static void BlendColor(const __context_RGBAValue&);
	static __context_RGBAValue BlendColor(void);
};

} // namespace context
//]

