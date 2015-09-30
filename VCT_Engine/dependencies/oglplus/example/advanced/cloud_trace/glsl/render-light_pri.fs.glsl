/*  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330
uniform sampler2DRect RaytraceOutput;

in vec2 vertTexCoord;
out vec3 fragColor;

float decode_light_pri(vec4 rt_data);

void main(void)
{
	vec4 rt = texture(RaytraceOutput, vertTexCoord);
	float lp = decode_light_pri(rt);
	fragColor = vec3(lp);
}
