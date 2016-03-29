#version 430

out vec3 texCoord;

layout(location = 0) in vec3 vertexTexCoord;

uniform uint volumeDimension;
uniform vec3 sceneMinPoint;
uniform vec3 voxelSize;

void main()
{
	ivec3 position = ivec3
	(
		gl_VertexID / (volumeDimension * volumeDimension),
		(gl_VertexID / volumeDimension) % volumeDimension,
		gl_VertexID % volumeDimension
	);

	vec3 worldPosition = vec3
	(
		position.x * voxelSize.x,
		position.y * voxelSize.y,
		position.z * voxelSize.z
	);
	worldPosition += vec3(voxelSize / 2.0f);
	worldPosition += sceneMinPoint;

	gl_Position = vec4(worldPosition, 1.0f);
	float volumeDimensionF = float(volumeDimension);

	texCoord = vec3
	(
		position.x / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF),
		position.y / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF),
		position.z / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF)
	);

	texCoord.z = 1.0f - texCoord.z;
}