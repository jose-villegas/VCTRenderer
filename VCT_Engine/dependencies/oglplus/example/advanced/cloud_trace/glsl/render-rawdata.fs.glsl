/*  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330
uniform sampler2DRect RaytraceOutput;
uniform float Near, Far;

in vec2 vertTexCoord;
out vec4 fragColor;

void main(void)
{
	vec4 rt = texture(RaytraceOutput, vertTexCoord);
	rt.x /= (Far-Near);
	rt.y /= (Far-Near);
	fragColor = rt;
}
