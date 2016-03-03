#version 420

layout(r32ui) uniform coherent uimageBuffer voxelPosition;
layout(r32ui) uniform volatile uimage3D voxelAlbedo;
layout(r32ui) uniform volatile uimage3D voxelNormal;

layout(binding = 0) uniform atomic_uint voxel_index;

uniform sampler2D diffuseMap;
uniform uint voxelTextureSize;

in Voxel
{
    vec3 position;
    vec3 normal;
    vec3 texCoord;
} In;

out vec4 fragColor;

vec4 convRGBA8ToVec4(uint val)
{
    return vec4( float((val & 0x000000FF)),
                 float((val & 0x0000FF00) >> 8U),
                 float((val & 0x00FF0000) >> 16U),
                 float((val & 0xFF000000) >> 24U));
}

uint convVec4ToRGBA8(vec4 val)
{
    return (uint(val.w) & 0x000000FF)   << 24U
           | (uint(val.z) & 0x000000FF) << 16U
           | (uint(val.y) & 0x000000FF) << 8U
           | (uint(val.x) & 0x000000FF);
}

uint vec3ToUintXYZ10(uvec3 val)
{
    return (uint(val.z) & 0x000003FF)   << 20U
           | (uint(val.y) & 0x000003FF) << 10U
           | (uint(val.x) & 0x000003FF);
}

uint imageAtomicRGBA8Avg(layout(r32ui) volatile uimage3D img,
                         ivec3 coords,
                         vec4 val)
{
    val.rgb *= 255.0f; // Optimise following calculations
    uint newVal = convVec4ToRGBA8(val);
    uint prevStoredVal = 0;
    uint curStoredVal;
    vec4 currVal;

    // Loop as long as destination value gets changed by other threads
    while ((curStoredVal = imageAtomicCompSwap(img, coords, prevStoredVal, newVal)) !=
            prevStoredVal)
    {
        prevStoredVal = curStoredVal;
        vec4 rval = convRGBA8ToVec4(curStoredVal);
        rval.xyz *= currVal.w; 		// Denormalize
        vec4 curValF = rval + val; 	// Add new value
        curValF.xyz /= curValF.w; 	// Renormalize
        newVal = convVec4ToRGBA8(curValF);
    }

    // currVal now contains the calculated color: now convert it to a
    // proper alpha-premultiplied version
    val = convRGBA8ToVec4(newVal);
    val.a = 255.0;
    newVal = convVec4ToRGBA8(val);
    imageStore(img, coords, uvec4(newVal));
    return newVal;
}

void main()
{
    uvec3 voxel = uvec3(floor(In.position * voxelTextureSize));
    vec4 diffuseColor = texture(diffuseMap, In.texCoord.xy);
    vec4 normal = vec4(normalize(In.normal) * 0.5f + 0.5f, 1.0f);
    uint voxelIndex = atomicCounterIncrement(voxel_index);
    memoryBarrier();
    // store voxel position
    imageStore(voxelPosition, int(voxelIndex), uvec4(vec3ToUintXYZ10(voxel)));
    // average voxel properties
    imageAtomicRGBA8Avg(voxelAlbedo, ivec3(voxel), diffuseColor);
    imageAtomicRGBA8Avg(voxelNormal, ivec3(voxel), normal);
}