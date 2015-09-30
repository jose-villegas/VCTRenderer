/*
 *  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_texture_swizzle_tuple

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
class TextureSwizzleTuple
{
public:
	TextureSwizzleTuple(void); /*<
	No swizzling.
	>*/

	TextureSwizzleTuple(
		__TextureSwizzle mode_r,
		__TextureSwizzle mode_g,
		__TextureSwizzle mode_b,
		__TextureSwizzle mode_a
	); /*<
	Specifies modes for all texture components/coordinates.
	>*/

	TextureSwizzleTuple& SwizzleR(__TextureSwizzle mode); /*<
	Sets the swizzle value for the red component.
	>*/
	TextureSwizzleTuple& Red(__TextureSwizzle mode);
	__TextureSwizzle SwizzleR(void) const; /*<
	Returns the swizzle value for the red component.
	>*/
	__TextureSwizzle Red(void) const;

	TextureSwizzleTuple& SwizzleG(__TextureSwizzle mode); /*<
	Sets the swizzle value for the green component.
	>*/
	TextureSwizzleTuple& Green(__TextureSwizzle mode);
	__TextureSwizzle SwizzleG(void) const; /*<
	Returns the swizzle value for the green component.
	>*/
	__TextureSwizzle Green(void) const;

	TextureSwizzleTuple& SwizzleB(__TextureSwizzle mode); /*<
	Sets the swizzle value for the blue component.
	>*/
	TextureSwizzleTuple& Blue(__TextureSwizzle mode);
	__TextureSwizzle SwizzleB(void) const; /*<
	Returns the swizzle value for the blue component.
	>*/
	__TextureSwizzle Blue(void) const;

	TextureSwizzleTuple& SwizzleA(__TextureSwizzle mode); /*<
	Sets the swizzle value for the alpha component.
	>*/
	TextureSwizzleTuple& Alpha(__TextureSwizzle mode);
	__TextureSwizzle SwizzleA(void) const; /*<
	Returns the swizzle value for the alpha component.
	>*/
	__TextureSwizzle Alpha(void) const;

	TextureSwizzleTuple& Swizzle(
		__TextureSwizzleCoord coordinate,
		__TextureSwizzle mode
	); /*<
	Sets the swizzle [^mode] for the specified [^coordinate].
	>*/
	__TextureSwizzle Swizzle(__TextureSwizzleCoord coordinate) const; /*<
	Returns the swizzle mode for the specified [^coordinate].
	>*/
};
#endif

//]

