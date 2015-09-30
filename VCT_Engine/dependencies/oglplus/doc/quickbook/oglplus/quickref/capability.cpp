/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_capability_ex_1
__Context gl;

gl.Enable(__Capability::DepthTest);
gl.Disable(__Capability::CullFace);

gl.Enable(__Functionality::ClipDistance, 0);
gl.Disable(__Functionality::ClipDistance, 1);
//]

//[oglplus_capability_ex_ss

__Capability::DepthTest << true; /*< Enables depth test. >*/
Capability::StencilTest << false; /*< Disables stencil test. >*/

+Capability::CullFace; /*< Enables face culling. >*/
-Capability::Blend; /*< Disables blending. >*/

for(int i=0; i<4; ++i)
{
	(__Functionality::ClipDistance | i) << true; /*<
	Enables the [^i]th clipping plane.
	>*/
}
//]

