/*
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330

float SpectrumValue(vec3 coord);

in vec3 vertTexCoord;
in float vertValue;
in float vertHighlight;
in float vertSelection;

out vec3 fragColor;

void main(void)
{
	float Light = clamp(0.2+sqrt(vertValue), 0.0, 2.0);
	float Strips = (int(vertTexCoord.y)%2)*0.1+(int(vertTexCoord.y*10)%2)*0.2;
	fragColor = mix(
		mix(
			mix(
				vec3(0.2, 0.1+Strips, 1.0),
				vec3(1.0, 0.1+Strips, 0.2),
				vertValue
			),
			vec3(1.0, 1.0, 1.0),
			vertSelection*0.2
		)*Light,
		vec3(1.0, 1.0, 1.0),
		vertHighlight
	);
}

