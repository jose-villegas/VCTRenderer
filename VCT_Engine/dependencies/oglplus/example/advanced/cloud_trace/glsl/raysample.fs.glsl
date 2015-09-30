/*  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330
#define MaxClouds 1024
#define N 64
#define K 2

uniform sampler3D CloudTex;
uniform int CloudCount;
layout (std140)
uniform CloudBlock
{
	mat4 CloudMatrices[MaxClouds];
};

vec4 sphere_geom(int idx)
{
	return transpose(CloudMatrices[idx])[3];
}

vec3 sphere_para(int idx)
{
	return CloudMatrices[idx][3].xyz;
}

mat3 sphere_rot(int idx)
{
	return mat3(CloudMatrices[idx]);
}

vec2 ray_sphere_hit(vec3 o, vec3 d, int idx)
{
	vec4 s = sphere_geom(idx);
	float r = s.w;
	vec3 v = o-s.xyz;
	float dv = dot(d,v);
	float vv = dot(v,v);
	float rr = r*r;
	float x = dv*dv-vv+rr;
	return vec2(dv, x);
}

vec3 sphere_point_coord(vec3 x, int idx)
{
	vec4 s = sphere_geom(idx);
	mat3 r = sphere_rot(idx);
	x -= s.xyz;
	x *= 0.5;
	x /= s.w;
	x = r*x;
	x += 0.5;
	return x;
}

const float maxt = 1e9;
float tmin[K], tmax[K];
vec3 hits[K*N];
vec3 wc0[K*N], wc1[K*N], tc0[K*N], tc1[K*N];

int find_hits(int k, vec3 ori, vec3 ray)
{
	tmin[k] = maxt;
	tmax[k] = 0;
	int n = 0, i = 0;
	while((i < CloudCount) && (n < N))
	{
		vec2 h = ray_sphere_hit(ori, ray, i);
		if(h.y >= 0)
		{
			h.y = sqrt(h.y);
			float tn = -h.x-h.y;
			float tf = -h.x+h.y;
			if(tf >= 0.0)
			{
				tn = max(0, tn);
				tmin[k] = min(tmin[k], tn);
				tmax[k] = max(tmax[k], tf);
				hits[k*N+n] = vec3(i, tn, tf);
				wc0[k*N+n] = ori+ray*tn;
				wc1[k*N+n] = ori+ray*tf;
				tc0[k*N+n] = sphere_point_coord(wc0[k*N+n], i);
				tc1[k*N+n] = sphere_point_coord(wc1[k*N+n], i);
				++n;
			}
		}
		++i;
	}
	return n;
}

struct rs_data
{
	float den;
	float age;
	float tstep;
	float tmin;
	float tmax;
};

rs_data sample_ray(int k, int n, float t, float dc)
{
	rs_data res;
	res.den = 0.0;
	res.age = 0.0;
	res.tstep = 1.0;
	res.tmin = tmin[k];
	res.tmax = tmax[k];

	for(int i=0; i<n; ++i)
	{
		float tn = hits[k*N+i][1];
		float tf = hits[k*N+i][2];
		float td = tf-tn;
		float tm = 0.001;
		if((td > tm) && (tn <= t) && (tf >= t))
		{
			int id = int(hits[k*N+i][0]);
			float ssc = (t-tn)/td;
			vec3 sp = sphere_para(id);
			vec3 tc = mix(tc0[k*N+i], tc1[k*N+i], ssc);
			float ssd = max(texture(CloudTex, tc).r - sp[0], 0.0);
			float sd = ssd * dc * sp[1];
			res.den += sd;

			vec3 tcd = abs(tc0[k*N+i]-tc1[k*N+i]);
			vec3 tsn = tcd*textureSize(CloudTex, 0);
			float nsam = ceil(max(max(tsn.x, tsn.y), tsn.z));

			res.age += sp[2]*sd;
			res.tstep = min(res.tstep, td / nsam);
		}
		if(tn > t)
		{
			res.tmin = min(res.tmin, tn);
		}
		if(tf < t)
		{
			res.tmax = max(res.tmax, tf);
		}
	}
	res.age = res.age/mix(1.0, res.den, sign(res.den));
	res.den = min(res.den, 1.0);
	return res;
}
