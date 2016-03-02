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

void imageAtomicRGBA8Avg(layout(r32ui) coherent volatile uimage3D imgUI, ivec3 coords, vec4 val)
{
    val.rgb *= 255.0f; //Optimise following calculations
    uint newVal = convVec4ToRGBA8(val);
    uint prevStoredVal = 0;
    uint curStoredVal;

    //Loop as long as destination value gets changed by other threads
    while ( (curStoredVal = imageAtomicCompSwap(imgUI , coords, prevStoredVal ,
                            newVal)) != prevStoredVal)
    {
        prevStoredVal = curStoredVal;
        vec4 rval = convRGBA8ToVec4(curStoredVal);
        rval.xyz = (rval.xyz * rval.w);
        //Denormalize
        vec4 curValF = rval + val;
        //Add new value
        curValF.xyz /= (curValF.w);
        //Renormalize
        newVal = convVec4ToRGBA8(curValF);
    }
}

void main()
{
	
}