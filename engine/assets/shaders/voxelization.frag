# version 430

in GeometryOut
{
    vec3 position;
    vec3 normal;
    vec3 texCoord;
    flat int selectedAxis;
    flat vec4 triangleAABB;
} In;

layout (location = 0) out vec4 fragColor;
layout (pixel_center_integer) in vec4 gl_FragCoord;


layout(r32ui) uniform volatile uimage3D voxelAlbedo;

layout(binding = 0) uniform atomic_uint voxelIndex;

uniform struct Material
{
    vec3 diffuse;
} material;

uniform sampler2D diffuseMap;
uniform uint volumeDimension;

vec4 convRGBA8ToVec4(uint val) {
    return vec4( float((val & 0x000000FF)), 
                 float((val & 0x0000FF00) >> 8U), 
                 float((val & 0x00FF0000) >> 16U), 
                 float((val & 0xFF000000) >> 24U));
}

uint convVec4ToRGBA8(vec4 val) {
    return (uint(val.w) & 0x000000FF)   << 24U
            | (uint(val.z) & 0x000000FF) << 16U
            | (uint(val.y) & 0x000000FF) << 8U 
            | (uint(val.x) & 0x000000FF);
}

uint imageAtomicRGBA8Avg(layout(r32ui) volatile uimage3D img, 
                         ivec3 coords,
                         vec4 newVal) {
    newVal.xyz *= 255.0; // Optimise following calculations
    uint newValU = convVec4ToRGBA8(newVal);
    uint lastValU = 0; 
    uint currValU;
    vec4 currVal;
    uint numIterations = 0;
    // Loop as long as destination value gets changed by other threads
    while((currValU = imageAtomicCompSwap(img, coords, lastValU, newValU))
          != lastValU
          && numIterations < 100) {
        lastValU = currValU;

        currVal = convRGBA8ToVec4(currValU);
        currVal.xyz *= currVal.a; // Denormalize

        currVal += newVal; // Add new value
        currVal.xyz /= currVal.a; // Renormalize

        newValU = convVec4ToRGBA8(currVal);

        ++numIterations;
    }

    // currVal now contains the calculated color: now convert it to a proper alpha-premultiplied version
    newVal = convRGBA8ToVec4(newValU);
    newVal.a = 255.0;
    newValU = convVec4ToRGBA8(newVal);

    imageStore(img, coords, uvec4(newValU));

    return newValU;
}

void main()
{
    if( In.position.x < In.triangleAABB.x || In.position.y < In.triangleAABB.y || 
		In.position.x > In.triangleAABB.z || In.position.y > In.triangleAABB.w )
	{
		discard;
	}

	uvec3 temp = uvec3( gl_FragCoord.x, gl_FragCoord.y, volumeDimension * gl_FragCoord.z ) ;
	uvec3 texcoord;

	if( In.selectedAxis == 0 )
	{
	    texcoord.x = volumeDimension - temp.z;
		texcoord.z = temp.x;
		texcoord.y = temp.y;
	}
	else if( In.selectedAxis == 1 )
    {
	    texcoord.z = temp.y;
		texcoord.y = volumeDimension - temp.z;
		texcoord.x = temp.x;
	}
	else
	{
		texcoord = temp;
	}

	vec3 normal = In.normal;
	vec4 albedo = texture( diffuseMap, In.texCoord.xy );
	albedo = vec4(albedo.rgb * material.diffuse, albedo.a);

	uint voxel = atomicCounterIncrement(voxelIndex);
	memoryBarrier();

	imageAtomicRGBA8Avg(voxelAlbedo, ivec3(texcoord.xyz), albedo);
}