/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_bitfield_example_1

__Context gl;

gl.Clear(__ClearBit::ColorBuffer); /*<
Initialization of a __Bitfield from a single enumeration value.
>*/

gl.Clear(
	__ClearBit::ColorBuffer|
	__ClearBit::DepthBuffer|
	__ClearBit::StencilBuffer
); /*<
Initialization from a set of enumeration values.
>*/

gl.Clear({
	__ClearBit::ColorBuffer,
	__ClearBit::DepthBuffer,
	__ClearBit::StencilBuffer
}); /*<
Initialization from an [^std::initializer_list] of enumeration values.
>*/

if(gl.ProfileMask().Has(__ContextProfileBit::Compatibility)) /*<
Testing if a particular bit is set
>*/
{
	/* ... */
}

//]

