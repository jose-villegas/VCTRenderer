/*  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330

uniform mat3 RayMatrix;
uniform vec4 ClipPlane[4];
uniform bool ClipTiles;

in vec2 Position;
out vec3 vertRay;

void main(void)
{
	gl_Position = vec4(Position, 0, 1);
	if(ClipTiles)
	{
		for(int c=0; c!=4; ++c)
		{
			gl_ClipDistance[c] = dot(ClipPlane[c], gl_Position);
		}
	}
	vertRay = RayMatrix * vec3(Position, 1);
}
