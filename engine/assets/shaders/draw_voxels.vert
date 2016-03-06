#version 430

out vec4 albedo;

uniform int volumeDimension;
uniform sampler3D voxelAlbedo;

void main()
{
	// voxel center
	vec3 position = vec3
	(
		gl_VertexID % volumeDimension,
		gl_VertexID / ( volumeDimension * volumeDimension ),
		( gl_VertexID / volumeDimension ) % volumeDimension
	);

	albedo = texture(voxelAlbedo, position / volumeDimension);
	gl_Position = vec4(position - volumeDimension * 0.5f, 1.0f);
}