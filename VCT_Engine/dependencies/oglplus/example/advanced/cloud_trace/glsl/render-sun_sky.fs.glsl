/*  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330
uniform sampler2DRect RaytraceOutput;
uniform float Near, Far;
uniform float PlanetRadius, AtmThickness;
uniform vec3 LightPos;

const vec3 LightColor = vec3(1, 1, 1);
const vec3 AirColor = vec3(0.28, 0.38, 0.62);
const vec3 HazeColor = vec3(0.75, 0.77, 0.85);

float decode_dist_first(vec4 rt_data);
float decode_dist_final(vec4 rt_data);
float decode_density(vec4 rt_data);
float decode_light_pri(vec4 rt_data);
float decode_light_sec(vec4 rt_data);

in vec3 vertRay;
in vec2 vertTexCoord;

out vec3 fragColor;

float atm_intersection(vec3 v)
{
	vec3 c = vec3(0.0, -PlanetRadius, 0.0);
	float r = PlanetRadius + AtmThickness;
	float c_c = dot(-c, -c);
	float v_c = dot( v, -c);
	return (-v_c + sqrt(v_c*v_c - c_c + r*r))/AtmThickness;
}

float dist_diff(vec2 offs, float first)
{
	vec4 rt = texture(RaytraceOutput, vertTexCoord+offs);
	float dd = first - decode_dist_first(rt);
	float da = abs(dd*5);
	return sign(dd)*da*exp(-da);
}

void main(void)
{
	vec4 rt = texture(RaytraceOutput, vertTexCoord);
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 ld = normalize(LightPos);
	vec3 rd = normalize(vertRay);

	float lt = decode_light_pri(rt);
	float lr = dot(ld, rd);
	float ur = dot(up, rd);
	float ul = dot(up, ld);
	float ai = atm_intersection(rd);
	float hr = (1.0-abs(ur));
	float iai = clamp(1.4/ai, 0, 1);
	float lai = log(ai);
	float ctl = pow(max(lr+0.3, 0.0), 2);
	float crl = mix(0.7, 1.1, decode_light_sec(rt));

	float cd = decode_density(rt);
	float ft = decode_dist_first(rt);
	float fl = decode_dist_final(rt);
	float id = (fl - Far/2);
	id /= (Far - Far/2);
	id = exp(-id)*(1-id);

	lt *= mix(mix(0.2, 0.9, id), 1.0, crl);

	float dd = 0;
	dd += dist_diff(vec2(-1,-1), ft);
	dd += dist_diff(vec2(-1, 0), ft);
	dd += dist_diff(vec2(-1, 1), ft);
	dd += dist_diff(vec2( 0,-1), ft);
	dd += dist_diff(vec2( 0, 1), ft);
	dd += dist_diff(vec2( 1,-1), ft);
	dd += dist_diff(vec2( 1, 0), ft);
	dd += dist_diff(vec2( 1, 1), ft);

	float ao = 0.03*dd*cd*(1.0-lt*crl)*id;

	float dc = (0.8+0.2*lt)*id*pow(cd, 0.25);

	vec3 Air1 =
		mix(HazeColor, AirColor, iai)*
		sqrt(max(ul+0.3, 0.0))*
		mix(0.9, 1.0, clamp(abs(lr+0.6), 0.2, 1.0));
	Air1 *= crl;

	vec3 Air2 =
		max(LightColor-AirColor*lai*crl, vec3(0))*pow(
			max(abs(lr+mix(0.00, 0.57, iai))-mix(0.00, 0.48, iai), 0.0),
			mix(0.25, 4.0, mix(hr, lai, 0.2))
		)*hr;
	Air2 *= crl;

	vec3 Air3 =
		(LightColor-AirColor*lai*0.4)*
		pow(max(lr+mix(0.0015, 0.0004, iai), 0.0), mix(256, 1024, hr));

	vec3 CloudsDk = (LightColor-AirColor*lai*mix(0.2, 0.4, max(-lr, 0)))*sqrt(max(ul+0.7-lr*0.2, 0.0))*0.70;
	vec3 CloudsLt = (LightColor-AirColor*lai*mix(0.2, 0.4, abs(lr+0.1)))*sqrt(1.0+ul*0.5);

	vec3 CloudsLi = (LightColor-AirColor*mix(1.0-ul, lai, 0.4))*2.60;

	vec3 Air = Air1+Air2*(1-cd*0.7)+Air3*(1-pow(cd, 2));
	vec3 Clouds = mix(
		CloudsLi,
		mix(CloudsDk, CloudsLt, clamp(lt+ao, 0, 1)),
		mix(1.0, cd, min(ctl, 1.0))
	);

	fragColor = mix(Air, Clouds, clamp(dc, 0, 1));
}
