/*  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330

uniform vec3 LightPos;
uniform float HighLight;
uniform float AmbiLight;
uniform float UnitOpacity;
uniform float UnitAttenuation;
uniform float Near, Far, CrepRayFar;
uniform uint CrepRaySam;

in vec3 vertRay;
out vec4 fragColor;

const float maxt = 1e9;

float tmin[2], tmax[2];

int find_hits(int k, vec3 ori, vec3 ray);


struct rs_data
{
	float den;
	float age;
	float tstep;
	float tmin;
	float tmax;
};
rs_data sample_ray(int k, int n, float t, float dc);

vec4 encode_rt_data(
	float dist_first,
	float dist_final,
	float light_pri,
	float light_sec,
	float density,
	float age
);

void main(void)
{
	const vec3 ori0 = vec3(0);
	vec3 ray0 = normalize(vertRay);
	vec3 ld = normalize(LightPos);
	int n0 = find_hits(0, ori0, ray0);
	float t0 = tmin[0];
	tmin[0] = maxt;
	float tfirst = maxt, tlast = 0;
	float den0 = 0.0;
	float age = 0.0;

	while((t0 < tmax[0]) && (den0 < 1.0))
	{
		rs_data sr0 = sample_ray(0, n0, t0, UnitOpacity);
		if(sr0.den > 0.0)
		{
			age = sr0.age;
			den0 += sr0.den;
			tfirst = min(tfirst, t0);
			tlast = t0;
		}
		if(sr0.tstep < 1.0)
		{
			t0 += sr0.tstep;
		}
		else
		{
			t0 = sr0.tmin;
			tmin[0] = maxt;
		}
	}
	den0 = min(den0, 1.0);
	tfirst = max(tfirst, 0);
	float lt = mix(AmbiLight, HighLight, (1-den0)*(dot(ray0, ld)+1)*0.5);
	float dist = Far;
	if(den0 > 0.0)
	{
		t0 = tlast;
		tmax[0] = 0.0;
		while(t0 >= tfirst)
		{
			rs_data sr0 = sample_ray(0, n0, t0, UnitOpacity);

			if(sr0.tstep < 1.0)
			{
				t0 -= sr0.tstep;
			}
			else
			{
				t0 = min(sr0.tmax, t0-0.01);
				tmax[0] = 0.0;
			}


			vec3 ori1 = ori0+ray0*t0;
			vec3 ray1 = normalize(LightPos-ori1);
			int n1 = find_hits(1, ori1, ray1);

			float t1 = tmin[1];
			tmin[1] = maxt;
			float den1 = 0.0;
			while((t1 < tmax[1]) && (den1 < 1.0))
			{
				float mult = clamp(t1, 1, 8);
				rs_data sr1 = sample_ray(1, n1, t1, UnitAttenuation*mult);
				den1 += sr1.den;

				float ss = sr1.tstep*mult;
				if(ss < 1.0)
				{
					t1 += ss;
				}
				else
				{
					t1 = sr1.tmin;
					tmin[1] = maxt;
				}
			}
			den1 = min(den1, 1.0);
			lt = mix(lt, mix(HighLight, mix(lt, AmbiLight, sr0.den), den1), sr0.den);
			age = mix(age, sr0.age, sr0.den);
			dist = mix(dist, t0, sr0.den);
		}
	}
	lt *= 1.414;
	age = clamp(age, 0, 0.999);
	float cden = den0;
	den0 = 0.0;

	float crlc = CrepRayFar / Far;
	float icrs = 1.0 / float(CrepRaySam);
	float crl = 0.0;
	float ts0 = (Far - Near) * icrs * crlc;

	float tm0[2];
	tm0[0] = min(max(tfirst, tlast), Far)*crlc;
	tm0[1] = Far*crlc;
	t0 = Near*crlc;

	for(int p=0; p!=2; ++p)
	{
		float mult = p+1;
		float sd = 0.5*mult;
		while(t0 < tm0[p])
		{
			rs_data sr0 = sample_ray(0, n0, t0, 0.1);
			den0 = mix(den0+sr0.den, cden, p);

			if(den0 >= 1.0) break;

			vec3 ori1 = ori0+ray0*t0;
			vec3 ray1 = normalize(LightPos-ori1);
			int n1 = find_hits(1, ori1, ray1);

			float den1 = 0.0;
			float tm1 = min(max(tmin[1], tmax[1]), Far*crlc);
			float ts1 = ts0*2*mult;
			float t1 = tmin[1];

			while((t1 < tm1) && (den1 < 1.0))
			{
				rs_data sr1 = sample_ray(1, n1, t1, sd);
				den1 += sr1.den;
				t1 += ts1;
			}
			den1 = min(den1, 1.0);

			crl += (1.0 - den1)*(1.0 - den0);

			t0 += ts0;
		}
	}
	crl *= icrs;

	fragColor = encode_rt_data(tfirst, dist, lt, crl, cden, age);
}
