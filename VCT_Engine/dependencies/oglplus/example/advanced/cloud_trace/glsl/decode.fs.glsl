/*  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330

float unpack_hi(float enc);
float unpack_lo(float enc);

float decode_dist_first(vec4 rt_data)
{
	return rt_data.x;
}

float decode_dist_final(vec4 rt_data)
{
	return rt_data.y;
}

float decode_light_pri(vec4 rt_data)
{
	return unpack_hi(rt_data.w);
}

float decode_light_sec(vec4 rt_data)
{
	return unpack_lo(rt_data.w);
}

float decode_density(vec4 rt_data)
{
	return unpack_hi(rt_data.z);
}

float decode_age(vec4 rt_data)
{
	return unpack_lo(rt_data.z);
}

