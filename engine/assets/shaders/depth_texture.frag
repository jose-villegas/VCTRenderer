#version 430

layout(location = 0) out vec4 outColor;

in vec4 position;

uniform vec2 exponents = vec2(40.0f, 20.0f);

vec2 WarpDepth(float depth)
{
    // rescale depth into [-1, 1]
    depth = 2.0 * depth - 1.0;
    float pos =  exp( exponents.x * depth);
    float neg = -exp(-exponents.y * depth);
	
    return vec2(pos, neg);
}

vec4 ShadowDepthToEVSM(float depth)
{
	vec2 warpedDepth = WarpDepth(depth);
	return vec4(warpedDepth.xy, warpedDepth.xy * warpedDepth.xy);
}

void main()
{
	outColor = ShadowDepthToEVSM(gl_FragCoord.z);
}