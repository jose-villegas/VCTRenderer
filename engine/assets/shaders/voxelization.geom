#version 420

in vec3 position;
in vec3 texCoord;
in vec3 normal;

out Voxel 
{
	vec3 position;
	vec3 normal;
	vec3 texCoord;
} voxel;

uniform mat4 voxelGridTransform;
uniform mat4 voxelGridInverseTransform;
uniform vec3 voxelGridSize;
uniform mat4 viewProjections[3];

uint CalculateProjectionAxis()
{
	const vec3 worldAxes[3] = vec3[3]
	(	
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0),
		vec3(0.0, 0.0, 1.0)	
	);
	
	uint projectionAxis = 0;
	float maximumArea = 0.0f;

	for(uint i = 0; i < 3; ++i)
	{
		float area = abs(dot(normal, worldAxes[i]));

		if(area > maxArea)
		{
			maxArea = area;
			projectionAxis = i;
		}
	}

	return projectionAxis;
}

void main()
{
	const vec3 worldAxes[3] = vec3[3]
	(	
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0),
		vec3(0.0, 0.0, 1.0)	
	);
}