/*
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330

uniform vec3 Color;
uniform float Alpha;

out vec4 fragColor;

void main(void)
{
	fragColor = vec4(Color, Alpha);
}

