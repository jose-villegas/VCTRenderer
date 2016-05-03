#version 430
layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout(binding = 0, rgba8) uniform writeonly image3D voxelMipmap[6];
layout(binding = 5) uniform sampler3D voxelBase;

uniform int mipDimension;

const ivec3 anisoOffsets[] = ivec3[8]
(
	ivec3(1, 1, 1),
	ivec3(1, 1, 0),
	ivec3(1, 0, 1),
	ivec3(1, 0, 0),
	ivec3(0, 1, 1),
	ivec3(0, 1, 0),
	ivec3(0, 0, 1),
	ivec3(0, 0, 0)
);

void FetchTexels(ivec3 pos, inout vec4 val[8]) 
{
	for(int i = 0; i < 8; i++)
	{
		val[i] = texelFetch(voxelBase, pos + anisoOffsets[i], 0);
	}
}

void main()
{
	if(gl_GlobalInvocationID.x >= mipDimension ||
		gl_GlobalInvocationID.y >= mipDimension ||
		gl_GlobalInvocationID.z >= mipDimension) return;

	ivec3 writePos = ivec3(gl_GlobalInvocationID);
	ivec3 sourcePos = writePos * 2;
	vec4 values[8];

	FetchTexels(sourcePos, values);
	// x -
	imageStore(voxelMipmap[0], writePos, 
	(
		values[0] + values[4] * (1 - values[0].a) + 
		values[1] + values[5] * (1 - values[1].a) +
		values[2] + values[6] * (1 - values[2].a) +
		values[3] + values[7] * (1 - values[3].a)) * 0.25f
	);
	// x +
    imageStore(voxelMipmap[1], writePos, 
	(
		values[4] + values[0] * (1 - values[4].a) +
    	values[5] + values[1] * (1 - values[5].a) +
    	values[6] + values[2] * (1 - values[6].a) +
    	values[7] + values[3] * (1 - values[7].a)) * 0.25f
    );
	// y -
    imageStore(voxelMipmap[2], writePos, 
	(
		values[0] + values[2] * (1 - values[0].a) +
    	values[1] + values[3] * (1 - values[1].a) +
    	values[5] + values[7] * (1 - values[5].a) +
    	values[4] + values[6] * (1 - values[4].a)) * 0.25f
    );
	// y +
    imageStore(voxelMipmap[3], writePos, 
	(
		values[2] + values[0] * (1 - values[2].a) +
    	values[3] + values[1] * (1 - values[3].a) +
    	values[7] + values[5] * (1 - values[7].a) +
    	values[6] + values[4] * (1 - values[6].a)) * 0.25f
    );
	// z -
    imageStore(voxelMipmap[4], writePos, 
	(
		values[0] + values[1] * (1 - values[0].a) +
    	values[2] + values[3] * (1 - values[2].a) +
    	values[4] + values[5] * (1 - values[4].a) +
    	values[6] + values[7] * (1 - values[6].a)) * 0.25f
    );
	// z +
    imageStore(voxelMipmap[5], writePos, 
	(
		values[1] + values[0] * (1 - values[1].a) +
    	values[3] + values[2] * (1 - values[3].a) +
    	values[5] + values[4] * (1 - values[5].a) +
    	values[7] + values[6] * (1 - values[7].a)) * 0.25f
    );
}