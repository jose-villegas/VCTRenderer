#version 430
#extension GL_ARB_shader_image_load_store : require
layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout(binding = 0, rgba8) uniform image3D voxelComposite;
layout(binding = 1) uniform sampler3D voxelAlbedo;
layout(binding = 2) uniform sampler3D voxelNormal;
layout(binding = 3) uniform sampler3D voxelTexMipmap[6];

const float PI = 3.14159265f;
const float EPSILON = 1e-30;
uniform float maxTracingDistanceGlobal = 1.0f;
uniform int volumeDimension;
uniform uint checkBoundaries = 0;

// four cones
const vec3 propagationDirections[] =
{
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.5f, 0.866025f),
    vec3(0.754996f, 0.5f, -0.4330128f),
    vec3(-0.754996f, 0.5f, -0.4330128f)
};

const float diffuseConeWeights[] =
{
    PI / 3.0f,
    2.0f * PI / 9.0f,
    2.0f * PI / 9.0f,
    2.0f * PI / 9.0f,
};

vec4 TraceCone(vec3 position, vec3 direction, float aperture)
{
    uvec3 visibleFace;
    // in propagation mode we only trace the mip levels for 
    // speed and to avoid aliasing issues
    float anisoDimension = volumeDimension / 2.0f;
    visibleFace.x = (direction.x < 0.0) ? 0 : 1;
    visibleFace.y = (direction.y < 0.0) ? 2 : 3;
    visibleFace.z = (direction.z < 0.0) ? 4 : 5;
    // weight per axis for aniso sampling
    vec3 weight = direction * direction;
    // navigation
    float voxelSize = 1.0f / anisoDimension;
    // move one voxel further to avoid self collision
    float dst = voxelSize;
    float diameter = aperture * dst;
    vec3 samplePos = position + direction * dst;
    // control vars
    float mipLevel = 0.0f;
    // accumulated sample
    vec4 coneSample = vec4(0.0f);
    vec4 anisoSample = vec4(0.0f);
    
    // outside bounds
    if(samplePos.x < 0.0f || samplePos.y < 0.0f || samplePos.z < 0.0f
        || samplePos.x > 1.0f || samplePos.y > 1.0f || samplePos.z > 1.0f) 
    { 
        return coneSample; 
    }

    while(coneSample.a <= 1.0f && dst <= maxTracingDistanceGlobal)
    {
        // outisde bounds
        if (checkBoundaries > 0 && (samplePos.x < 0.0f || samplePos.y < 0.0f || samplePos.z < 0.0f
            || samplePos.x > 1.0f || samplePos.y > 1.0f || samplePos.z > 1.0f)) 
        { 
            break; 
        }

        mipLevel = log2(diameter * anisoDimension);
        mipLevel = max(mipLevel - 1.0f, 0.0f);
        // aniso sampling
        anisoSample = weight.x * textureLod(voxelTexMipmap[visibleFace.x], samplePos, mipLevel)
                    + weight.y * textureLod(voxelTexMipmap[visibleFace.y], samplePos, mipLevel)
                    + weight.z * textureLod(voxelTexMipmap[visibleFace.z], samplePos, mipLevel);
        // accumulate sampling
        coneSample += (1.0f - coneSample.a) * anisoSample;
        // move further into volume
        dst += max(diameter, voxelSize);
        diameter = dst * aperture;
        samplePos = direction * dst + position;
    }

    return coneSample;
}

vec4 CalculateIndirectLighting(vec3 position, vec3 normal)
{
    // move position further along normal, 1 voxel step, half dimension
    position = position + normal * (1.0f / (volumeDimension / 2.0f));
    // cummulative
    vec4 diffuseTrace = vec4(0.0f);
    // diffuse cone setup
    const float aperture = 1.0f;
    vec3 guide = vec3(0.0f, 1.0f, 0.0f);

    if (abs(dot(normal, guide)) == 1.0f)
    {
        guide = vec3(0.0f, 0.0f, 1.0f);
    }

    // Find a tangent and a bitangent
    vec3 right = normalize(guide - dot(normal, guide) * normal);
    vec3 up = cross(right, normal);

    for(int i = 0; i < 4; i++)
    {
        vec3 coneDirection = normal;
        coneDirection += propagationDirections[i].x * right + propagationDirections[i].z * up;
        coneDirection = normalize(coneDirection);

        diffuseTrace += TraceCone(position, coneDirection, aperture) * diffuseConeWeights[i];
    }

    return clamp(diffuseTrace, 0.0f, 1.0f);
}

vec3 EncodeNormal(vec3 normal)
{
    return normal * 0.5f + vec3(0.5f);
}

vec3 DecodeNormal(vec3 normal)
{
    return normal * 2.0f - vec3(1.0f);
}

void main()
{
    if(gl_GlobalInvocationID.x >= volumeDimension ||
        gl_GlobalInvocationID.y >= volumeDimension ||
        gl_GlobalInvocationID.z >= volumeDimension) return;

    ivec3 writePos = ivec3(gl_GlobalInvocationID);
    vec4 albedo = texelFetch(voxelAlbedo, writePos, 0);

    if(albedo.a < EPSILON) { return; }

    vec4 directLight = imageLoad(voxelComposite, writePos);
    // normal from voxelization
    vec3 normal = texelFetch(voxelNormal, writePos, 0).xyz;
    // normal is stored in 0-1 range, restore to -1 -> -1
    normal = normalize(DecodeNormal(normal));
    // calculate indirect lighting - first bounce onto the voxel texture
    vec3 position = vec3(writePos) / volumeDimension;
    vec4 indirectLighting = CalculateIndirectLighting(position, normal);
    // first bounce gi component only
    indirectLighting *= albedo;
    // first bounce + direct lighting
    vec4 radiance = directLight + indirectLighting;
    radiance.a = directLight.a;

    imageStore(voxelComposite, writePos, radiance);
}