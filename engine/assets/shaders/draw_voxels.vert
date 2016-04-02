#version 430

out vec4 albedo;

layout(binding = 0, rgba8) uniform readonly image3D voxelRadiance;

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

	albedo = imageLoad(voxelRadiance, ivec3(position));
	gl_Position = vec4(position - volumeDimension * 0.5f, 1.0f);
}