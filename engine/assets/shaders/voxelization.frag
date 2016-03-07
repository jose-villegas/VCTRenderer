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
                         vec4 val) {
    val.xyz *= 255.0; // Optimise following calculations
    uint newVal = convVec4ToRGBA8(val);
    uint prevStoredVal = 0; 
    uint curStoredVal;
    vec4 rval;
    // Loop as long as destination value gets changed by other threads
    while((curStoredVal = imageAtomicCompSwap(img, coords, prevStoredVal, newVal)) != prevStoredVal) {
        prevStoredVal = curStoredVal;

        rval = convRGBA8ToVec4(curStoredVal);
        rval.xyz *= rval.a; // Denormalize

        rval += val; // Add new value
        rval.xyz /= rval.a; // Renormalize

        newVal = convVec4ToRGBA8(rval);
    }

    // rval now contains the calculated color: now convert it to a proper alpha-premultiplied version
    val = convRGBA8ToVec4(newVal);
    val.a = 255.0;
    newVal = convVec4ToRGBA8(val);

    imageStore(img, coords, uvec4(newVal));

    return newVal;
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