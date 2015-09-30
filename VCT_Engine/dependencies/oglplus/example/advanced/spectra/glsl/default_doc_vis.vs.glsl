/*
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330

float SpectrumValue(vec3 coord);

uniform mat4 TransfMatrix;
uniform float SelectedTime, SelectionBegin, SelectionEnd;

in vec4 Position;
in vec2 TexCoord;

out vec3 vertTexCoord;
out float vertValue;
out float vertHighlight;
out float vertSelection;

void main(void)
{
	vertTexCoord = vec3(TexCoord, gl_InstanceID);
	vec4 OffsPos = Position;
	OffsPos.x += gl_InstanceID;
	OffsPos.y = SpectrumValue(vertTexCoord);
	gl_Position = TransfMatrix * OffsPos;
	vertValue = OffsPos.y*2.0;
	vertHighlight = clamp(pow(exp(-abs(SelectedTime-OffsPos.x)*250.0), 4.0), 0.0, 1.0);
	vertSelection = exp(-250.0*(OffsPos.x-SelectionBegin)*(OffsPos.x-SelectionEnd));
	vertSelection = pow(clamp(vertSelection, 0.0, 1.0), 4.0);
}

