/*
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330

float SpectrumValue(vec3 coord);

uniform mat4 ProjectionMatrix, CameraMatrix;
uniform float SelectedTime;

in vec4 Position;
in vec2 TexCoord;

out vec3 vertTexCoord;
out float vertValue;
out float vertHighlight;

void main(void)
{
	vertTexCoord = vec3(TexCoord, gl_InstanceID);
	vec4 OffsPos = Position;
	OffsPos.x += gl_InstanceID;
	OffsPos.y = SpectrumValue(vertTexCoord);
	gl_Position = ProjectionMatrix * CameraMatrix * OffsPos;
	vertValue = OffsPos.y;
	vertHighlight = pow(exp(-abs(SelectedTime-OffsPos.x)*100.0), 2.0);
}

