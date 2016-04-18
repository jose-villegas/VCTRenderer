#version 430
#extension GL_ARB_shader_image_load_store : require

out vec4 albedo;

layout(binding = 0, rgba8) uniform readonly image3D voxelRadiance;

uniform uint volumeDimension;
uniform vec4 colorChannels;

void main()
{
	float volumeDimensionF = float(volumeDimension);

	vec3 position = vec3
	(
		gl_VertexID % volumeDimension,
		(gl_VertexID / volumeDimension) % volumeDimension,
		gl_VertexID / (volumeDimension * volumeDimension)
	);

	ivec3 texPos = ivec3(position);
	albedo = imageLoad(voxelRadiance, texPos);

	uvec4 channels = uvec4(floor(colorChannels));

	albedo = vec4(albedo.rgb * channels.rgb, albedo.a);
	// if no color channel is enabled but alpha is one, show alpha as rgb
	if(all(equal(channels.rgb, uvec3(0))) && channels.a > 0) 
	{
		albedo = vec4(albedo.a);
	}

	gl_Position = vec4(position, 1.0f);
}