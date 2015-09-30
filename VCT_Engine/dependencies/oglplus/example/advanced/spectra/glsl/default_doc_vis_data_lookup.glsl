/*
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330

uniform samplerBuffer SpectrumTex;
uniform int SpectrumSize;
uniform int SamplesPerUnit;

float SpectrumValue(vec3 coord)
{
	int row = int(SamplesPerUnit*(coord.z+coord.y));
	int col = int((SpectrumSize-1)*coord.x);

	return texelFetch(SpectrumTex, int(row*SpectrumSize+col)).r;
}

