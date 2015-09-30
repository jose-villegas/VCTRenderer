/*
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330

uniform mat4 TransfMatrix;

in vec4 Position;

void main(void)
{
	vec4 OffsPos = Position;
	OffsPos.x += gl_InstanceID;
	gl_Position = TransfMatrix * OffsPos;
}

