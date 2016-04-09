#version 430

layout(location = 0) out vec4 fragColor;

in vec2 texCoord;

layout(binding = 0) uniform sampler2D directLight;
layout(binding = 1) uniform sampler2D indirectLight;

void main()
{
	vec4 direct = texture(directLight, texCoord);
	vec4 indirect = texture(indirectLight, texCoord);

	fragColor = vec4((direct.rgb + indirect.rgb) * indirect.a, 1.0f);
}