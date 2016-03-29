#version 430

out vec3 texCoord;

layout(location = 0) in vec3 vertexTexCoord;

uniform uint volumeDimension;
uniform float voxelSize;

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
		(position.x - halfDimension + 0.5f) * voxelSize,
		(position.y - halfDimension + 0.5f) * voxelSize,
		(position.z - halfDimension + 0.5f) * voxelSize
	);

	gl_Position = vec4(worldPosition, 1.0f);
	float volumeDimensionF = float(volumeDimension);

	texCoord = vec3
	(
		position.x / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF),
		position.y / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF),
		position.z / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF)
	);
}