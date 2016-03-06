#version 430

out vec4 voxelColor;
out vec3 worldNormal;

out vec4 fragColor;

void main()
{
	if(voxelColor.a < 0.1f)
	{
		discard;
	}

	fragColor = vec4(voxelColor);
}