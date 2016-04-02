#version 430

out vec4 albedo;

layout(location = 0) in vec3 vertexTexCoord;
layout(binding = 0, rgba8) uniform sampler3D voxelRadiance;

uniform uint volumeDimension;

void main()
{
	float volumeDimensionF = float(volumeDimension);

	vec3 position = vec3
	(
		gl_VertexID % volumeDimension,
		(gl_VertexID / volumeDimension) % volumeDimension,
		gl_VertexID / (volumeDimension * volumeDimension)
	);

	vec3 texCoord = vec3
	(
		position.x / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF),
		position.y / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF),
		position.z / volumeDimensionF + 1.0f / (2.0f * volumeDimensionF)
	);

	albedo = texture(voxelRadiance, texCoord.xyz);
	gl_Position = vec4(position - volumeDimensionF * 0.5f, 1.0f);
}