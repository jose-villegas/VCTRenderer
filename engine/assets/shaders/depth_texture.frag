#version 430

layout(location = 0) out vec2 fragmentDepth;

in vec4 position;

void main()
{
	float depth = position.z / position.w;
	depth = depth * 0.5f + 0.5f;
	float depthSq = depth * depth;

	// adjust to avoid bias
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	depthSq += 0.25f * (dx * dx + dy * dy);

    fragmentDepth = vec2(depth, depthSq);
}