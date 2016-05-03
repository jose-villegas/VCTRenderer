#version 430
layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout(binding = 0, rgba8) uniform writeonly image3D voxelMipmapDst[6];
layout(binding = 5) uniform sampler3D voxelMipmapSrc[6];

uniform vec3 mipDimension;
uniform int mipLevel;

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

void FetchTexels(ivec3 pos, int dir, inout vec4 val[8]) 
{
	for(int i = 0; i < 8; i++)
	{
		val[i] = texelFetch(voxelMipmapSrc[dir], pos + anisoOffsets[i], mipLevel);
	}
}

void main()
{
	if(gl_GlobalInvocationID.x >= mipDimension.x ||
		gl_GlobalInvocationID.y >= mipDimension.y ||
		gl_GlobalInvocationID.z >= mipDimension.z) return;

	ivec3 writePos = ivec3(gl_GlobalInvocationID);
	ivec3 sourcePos = writePos * 2;
	// fetch values
	vec4 values[8];
	// x -
	FetchTexels(sourcePos, 0, values);
	imageStore(voxelMipmapDst[0], writePos, 
	(
		values[0] + values[4] * (1 - values[0].a) + 
		values[1] + values[5] * (1 - values[1].a) +
		values[2] + values[6] * (1 - values[2].a) +
		values[3] + values[7] * (1 - values[3].a)) * 0.25f
	);
	// x +
	FetchTexels(sourcePos, 1, values);
    imageStore(voxelMipmapDst[1], writePos, 
	(
		values[4] + values[0] * (1 - values[4].a) +
    	values[5] + values[1] * (1 - values[5].a) +
    	values[6] + values[2] * (1 - values[6].a) +
    	values[7] + values[3] * (1 - values[7].a)) * 0.25f
    );
	// y -	
	FetchTexels(sourcePos, 2, values);
    imageStore(voxelMipmapDst[2], writePos, 
	(
		values[0] + values[2] * (1 - values[0].a) +
    	values[1] + values[3] * (1 - values[1].a) +
    	values[5] + values[7] * (1 - values[5].a) +
    	values[4] + values[6] * (1 - values[4].a)) * 0.25f
    );
	// y +
	FetchTexels(sourcePos, 3, values);
    imageStore(voxelMipmapDst[3], writePos, 
	(
		values[2] + values[0] * (1 - values[2].a) +
    	values[3] + values[1] * (1 - values[3].a) +
    	values[7] + values[5] * (1 - values[7].a) +
    	values[6] + values[4] * (1 - values[6].a)) * 0.25f
    );
	// z -
	FetchTexels(sourcePos, 4, values);
    imageStore(voxelMipmapDst[4], writePos, 
	(
		values[0] + values[1] * (1 - values[0].a) +
    	values[2] + values[3] * (1 - values[2].a) +
    	values[4] + values[5] * (1 - values[4].a) +
    	values[6] + values[7] * (1 - values[6].a)) * 0.25f
    );
	// z +
	FetchTexels(sourcePos, 5, values);
    imageStore(voxelMipmapDst[5], writePos, 
	(
		values[1] + values[0] * (1 - values[1].a) +
    	values[3] + values[2] * (1 - values[3].a) +
    	values[5] + values[4] * (1 - values[5].a) +
    	values[7] + values[6] * (1 - values[7].a)) * 0.25f
    );
}