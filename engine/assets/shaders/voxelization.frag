#version 450
#extension GL_ARB_shader_image_load_store : require

in GeometryOut
{
    vec3 wsPosition;
    vec3 position;
    vec3 normal;
    vec3 texCoord;
    flat vec4 triangleAABB;
} In;

layout (location = 0) out vec4 fragColor;
layout (pixel_center_integer) in vec4 gl_FragCoord;

layout(binding = 0, r32ui) uniform volatile coherent uimage3D voxelAlbedo;
layout(binding = 1, r32ui) uniform volatile coherent uimage3D voxelNormal;
layout(binding = 2, rgba8) uniform volatile coherent image3D voxelEmission;

layout(binding = 3) uniform sampler2D diffuseMap;
layout(binding = 4) uniform sampler2D opacityMap;

uniform struct Material
{
    vec3 diffuse;
    vec3 emissive;
} material;

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

void imageAtomicRGBA8Avg(layout(r32ui) volatile coherent uimage3D grid, ivec3 coords, vec4 value)
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

vec3 EncodeNormal(vec3 n)
{
    vec2 enc = normalize(n.xy) * sqrt(-n.z * 0.5 + 0.5);
    return vec3(enc * 0.5 + 0.5, 0.0);
}

vec3 DecodeNormal(vec3 enc)
{
    vec4 nn = vec4(enc, 1) * vec4(2,2,0,0) + vec4(-1,-1,1,-1);
    float l = dot(nn.xyz, -nn.xyw);
    nn.z = l;
    nn.xy *= sqrt(l);
    return nn.xyz * 2.0f + vec3(0, 0, -1);
}

void main()
{
    if( In.position.x < In.triangleAABB.x || In.position.y < In.triangleAABB.y || 
		In.position.x > In.triangleAABB.z || In.position.y > In.triangleAABB.w )
	{
		discard;
	}

    const ivec3 zero = ivec3(0);
    // fragment albedo
    vec4 albedo = texture(diffuseMap, In.texCoord.xy);
    // albedo.rgb *= material.diffuse;

    float opacity = min(albedo.a, texture(opacityMap, In.texCoord.xy).r);
    // alpha cutoff
    if(opacity > 0.0f)
    {
        // albedo is in srgb space, bring back to linear
        albedo.rgb = material.diffuse * albedo.rgb;
        // premultiplied alpha
        albedo.rgb *= opacity;
        albedo.a = 1.0f;
        // bring normal to 0-1 range
        vec4 normal = vec4(EncodeNormal(normalize(In.normal)), 1.0f);
        ivec3 dimension = ivec3(volumeDimension);
        ivec3 position = ivec3(In.wsPosition);

        if(all(greaterThanEqual(position, zero)) && all(lessThan(position, dimension)))
        {
            // average normal per fragments sorrounding the voxel volume
            imageAtomicRGBA8Avg(voxelNormal, position, normal);
            // average albedo per fragments sorrounding the voxel volume
            imageAtomicRGBA8Avg(voxelAlbedo, position, albedo);
            // store emissive, no average operation since emissive is a single color
            if(any(greaterThan(material.emissive, vec3(0.0f))))
            {
                imageStore(voxelEmission, position, vec4(material.emissive, 1.0f));
            }
        }
    }
}