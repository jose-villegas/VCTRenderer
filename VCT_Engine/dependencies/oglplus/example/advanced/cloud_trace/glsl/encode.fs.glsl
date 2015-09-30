/*  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330

float pack_pair(float hi, float lo);

vec4 encode_rt_data(
	float dist_first,
	float dist_final,
	float light_pri,
	float light_sec,
	float density,
	float age
)
{
	return vec4(
		dist_first,
		dist_final,
		pack_pair(density, age),
		pack_pair(light_pri, light_sec)
	);
}
