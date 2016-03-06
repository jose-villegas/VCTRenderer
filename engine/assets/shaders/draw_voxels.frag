#version 430

in vec4 voxelColor;

out vec4 fragColor;

void main()
{
	if(voxelColor.a < 0.1f) discard;

	fragColor = vec4(voxelColor);
}