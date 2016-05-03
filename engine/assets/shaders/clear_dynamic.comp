#version 430
#extension GL_ARB_shader_image_load_store : require
layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout(binding = 0, rgba8) uniform image3D voxelAlbedo;
layout(binding = 1, rgba8) uniform writeonly image3D voxelNormal;
layout(binding = 2, rgba8) uniform writeonly image3D voxelEmissive;
layout(binding = 3) uniform sampler3D staticVoxelFlag;

const float EPSILON = 1e-30;

void main()
{
    int volumeDimension = imageSize(voxelAlbedo).x;

	if(gl_GlobalInvocationID.x >= volumeDimension ||
		gl_GlobalInvocationID.y >= volumeDimension ||
		gl_GlobalInvocationID.z >= volumeDimension) return;

    ivec3 writePos = ivec3(gl_GlobalInvocationID);

    // empty voxel
    if(imageLoad(voxelAlbedo, writePos).a < EPSILON) { return; }

    // static flag is true
    if(texelFetch(staticVoxelFlag, writePos, 0).r > EPSILON) { return; }

    // is a non-static voxel and the voxel in that position isn't empty
    // clear the volumnes with 0,0,0,0
    imageStore(voxelAlbedo, writePos, vec4(0.0));
    imageStore(voxelNormal, writePos, vec4(0.0));
    imageStore(voxelEmissive, writePos, vec4(0.0));
}