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

layout(binding = 0, r32ui) uniform uimage3D voxelAlbedo;
layout(binding = 1, r32ui) uniform uimage3D voxelNormal;

uniform struct Material
{
    vec3 diffuse;
} material;

uniform sampler2D diffuseMap;
uniform uint volumeDimension;

vec4 convRGBA8ToVec4(uint val)
{
    return vec4(float((val & 0x000000FF)), 
    float((val & 0x0000FF00) >> 8U), 
    float((val & 0x00FF0000) >> 16U), 
    float((val & 0xFF000000) >> 24U));
}

uint convVec4ToRGBA8(vec4 val)
{
    return (uint(val.w) & 0x000000FF) << 24U | 
    (uint(val.z) & 0x000000FF) << 16U | 
    (uint(val.y) & 0x000000FF) << 8U | 
    (uint(val.x) & 0x000000FF);
}

void imageAtomicRGBA8Avg(layout(r32ui) uimage3D grid, ivec3 coords, vec4 value)
{
    value.rgb *= 255.0;                 // optimize following calculations
    uint newVal = convVec4ToRGBA8(value);
    uint prevStoredVal = 0;
    uint curStoredVal;

    while((curStoredVal = imageAtomicCompSwap(grid, coords, prevStoredVal, newVal)) != prevStoredVal)
    {
        prevStoredVal = curStoredVal;
        vec4 rval = convRGBA8ToVec4(curStoredVal);
        rval.rgb = (rval.rgb * rval.a); // Denormalize
        vec4 curValF = rval + value;    // Add
        curValF.rgb /= curValF.a;       // Renormalize
        newVal = convVec4ToRGBA8(curValF);
    }
}

void main()
{
    if( In.position.x < In.triangleAABB.x || In.position.y < In.triangleAABB.y || 
		In.position.x > In.triangleAABB.z || In.position.y > In.triangleAABB.w )
	{
		discard;
	}

	uvec3 temp = uvec3( gl_FragCoord.xy, volumeDimension * gl_FragCoord.z ) ;
	uvec3 voxelPos;

	if( In.selectedAxis == 0 )
	{
	    voxelPos.x = volumeDimension - temp.z;
		voxelPos.z = temp.x;
		voxelPos.y = temp.y;
	}
	else if( In.selectedAxis == 1 )
    {
	    voxelPos.z = temp.y;
		voxelPos.y = volumeDimension - temp.z;
		voxelPos.x = temp.x;
	}
	else
	{
		voxelPos = temp;
	}

    // flip z tex coord
    voxelPos.z = volumeDimension - voxelPos.z;
    voxelPos = min(max(voxelPos, uvec3(0)), uvec3(volumeDimension - 1));

    // fragment albedo
    vec4 albedo = texture(diffuseMap, In.texCoord.xy);
    albedo.rgb *= material.diffuse;

    // bring normal to 0 -> 1 range
    vec4 normal = vec4(In.normal * 0.5f + 0.5f, 0.0f);

    // alpha cutoff
    if(albedo.a <= 0.5f) discard;

    // average albedo per fragments sorrounding the voxel volume
	imageAtomicRGBA8Avg(voxelAlbedo, ivec3(voxelPos), albedo);
    // average normal per fragments sorrounding the voxel volume
    imageAtomicRGBA8Avg(voxelNormal, ivec3(voxelPos), normal);
}