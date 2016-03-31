#version 430

layout(location = 0) out vec2 fragmentDepth;

in vec4 position;

void main()
{
	float depth = gl_FragCoord.z;
	
	float moment1 = depth;
	float moment2 = depth * depth;

	float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25f * (dx * dx + dy * dy);

	fragmentDepth = vec2(moment1, moment2);
}