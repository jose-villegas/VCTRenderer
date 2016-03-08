#version 430

out vec3 texCoord;

layout(location = 0) in vec3 vertexTexCoord;

uniform uint volumeDimension;
uniform float voxelSize;
uniform vec3 voxelGridMove;

void main()
{
	ivec3 position = ivec3
	(
		gl_VertexID / (volumeDimension * volumeDimension),
		(gl_VertexID / volumeDimension) % volumeDimension,
		gl_VertexID % volumeDimension
	);

	float halfDimension = float(volumeDimension) / 2.0f;

	vec3 worldPosition = vec3
	(
		position.x * voxelSize,
		position.y * voxelSize,
		position.z * voxelSize
	) + vec3(voxelSize / 2.0f) + voxelGridMove;

	gl_Position = vec4(worldPosition, 1.0f);
	float volumeDimensionF = float(volumeDimension);

	texCoord = vec3
	(
		position.x / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF),
		position.y / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF),
		position.z / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF)
	);
}