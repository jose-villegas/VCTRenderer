#version 430

layout(location = 0) out vec4 fragColor;

in vec2 texCoord;

const float PI = 3.14159265f;

uniform mat4 inverseProjectionView;
uniform vec3 cameraPosition;

uniform sampler2D gDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform sampler3D voxelTex;
uniform sampler3D voxelTexMipmap;
uniform float voxelScale;
uniform vec3 worldMinPoint;
uniform int volumeDimension;

uniform float maxTracingDistanceGlobal = 1.0f;
uniform float bounceStrength = 1.0f;
uniform float aoFalloff = 725.0f;
uniform float aoAlpha = 0.01f;
uniform uint enableAo = 1;

const vec3 mipOffset[] = 
{
    vec3(0.0f, 0.0f, 0.0f),
    vec3(1.0f / 6.0f, 0.0f, 0.0f),
    vec3(2.0f / 6.0f, 0.0f, 0.0f),
    vec3( 3.0f / 6.0f, 0.0f, 0.0f),
    vec3(4.0f / 6.0f, 0.0f, 0.0f),
    vec3(5.0f / 6.0f, 0.0f, 0.0f)
};

const vec3 diffuseConeDirections[] =
{
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.5f, 0.866025f),
    vec3(0.823639f, 0.5f, 0.267617f),
    vec3(0.509037f, 0.5f, -0.7006629f),
    vec3(-0.50937f, 0.5f, -0.7006629f),
    vec3(-0.823639f, 0.5f, 0.267617f)
};

const float diffuseConeWeights[] =
{
    PI / 4.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
    3.0f * PI / 20.0f,
};

vec3 WorldToVoxelSample(vec3 position)
{
    vec3 voxelPos = position - worldMinPoint;
    return voxelPos * voxelScale;
}

vec4 TraceCone(vec3 position, vec3 direction, float aperture, float maxTracingDistance, bool traceOcclusion)
{
    uvec3 visibleFace;
    visibleFace.x = (direction.x < 0.0) ? 0 : 1;
    visibleFace.y = (direction.y < 0.0) ? 2 : 3;
    visibleFace.z = (direction.z < 0.0) ? 4 : 5;
    // weight per axis for aniso sampling
    vec3 weight = abs(direction);
    // navigation
    float voxelSize = 1.0f / volumeDimension;
    // move one voxel further to avoid self collision
    float dst = voxelSize * 2.0f;
    float diameter = aperture * dst;
    vec3 samplePos = direction * dst + position;
    vec3 anisoPos = vec3(samplePos.x / 6.0f, samplePos.yz);
    // control vars
    float mipLevel = 0.0f;
    float anisoLevel = 0.0f;
    float mipMaxLevel = log2(volumeDimension);
    // accumulated sample
    vec4 coneSample = vec4(0.0f);
    vec4 baseColor = vec4(0.0f);
    vec4 anisoSample = vec4(0.0f);
    float occlusion = traceOcclusion ? 0.0f : 1.0f;

    while(coneSample.a <= 1.0f && dst <= maxTracingDistance && dst <= maxTracingDistanceGlobal)
    {
        // outisde bounds
        if (aperture < 0.3f && (samplePos.x < 0.0f || samplePos.y < 0.0f || samplePos.z < 0.0f
            || samplePos.x > 1.0f || samplePos.y > 1.0f || samplePos.z > 1.0f)) 
        { 
            break; 
        }

        mipLevel = clamp(log2(diameter * volumeDimension), 0.0f, mipMaxLevel);
        anisoLevel = max(mipLevel - 1.0f, 0.0f);
        // aniso sampling
        anisoSample = weight.x * textureLod(voxelTexMipmap, anisoPos + mipOffset[visibleFace.x], anisoLevel)
                    + weight.y * textureLod(voxelTexMipmap, anisoPos + mipOffset[visibleFace.y], anisoLevel)
                    + weight.z * textureLod(voxelTexMipmap, anisoPos + mipOffset[visibleFace.z], anisoLevel);

       if(mipLevel <= 1.0f)
        {
            baseColor = texture(voxelTex, samplePos);
            anisoSample = mix(baseColor, anisoSample, clamp(mipLevel, 0.0f, 1.0f));
        }
        // ambient occlusion lookup
        if (traceOcclusion && occlusion <= 1.0 && aoAlpha != 1.0f) 
        {
            occlusion += ((1.0f - occlusion) * anisoSample.a) / (1.0f + dst * aoFalloff);
        }
        // accumulate sampling
        coneSample += (1.0f - coneSample.a) * anisoSample;
        // move further into volume
        dst += max(diameter, voxelSize);
        diameter = dst * aperture;
        samplePos = direction * dst + position;
        anisoPos = vec3(samplePos.x / 6.0f, samplePos.yz);
    }

    return vec4(coneSample.rgb, occlusion);
}

vec3 PositionFromDepth()
{
    float z = texture(gDepth, texCoord).x * 2.0f - 1.0f;
    vec4 projected = vec4(texCoord * 2.0f - 1.0f, z, 1.0f);
    projected = inverseProjectionView * projected;
    return projected.xyz / projected.w;
}

vec4 CalculateIndirectLighting(vec3 position, vec3 normal, vec3 albedo, vec4 specular, bool ambientOcclusion)
{
    vec3 positionT = WorldToVoxelSample(position);
    vec3 cameraPosT = WorldToVoxelSample(cameraPosition);

    vec3 viewDirection = normalize(cameraPosT - positionT);
    vec3 coneDirection = reflect(-viewDirection, normal);

    vec4 specularTrace = vec4(0.0f);
    vec4 diffuseTrace = vec4(0.0f);

    // component greater than zero
    if(any(greaterThan(specular.rgb, specularTrace.rgb)))
    {
        // specular cone setup
        float factor = 1.0f / (specular.a * specular.a * 0.001953125f + 1.0f);
        float aperture = sin(acos(sqrt(0.11f / (factor + 0.11f))));
        specularTrace = TraceCone(positionT.xyz, coneDirection, aperture, 1.0f, false);
        specularTrace.rgb *= specular.rgb;
    }

    // component greater than zero
    if(any(greaterThan(albedo, diffuseTrace.rgb)))
    {
        // diffuse cone setup
        float aperture = 0.5055f;
        vec3 up = (normal.y * normal.y) > 0.95f ? vec3(0.0f, 0.0f, 1.0f) : vec3(0.0f, 1.0f, 0.0f);
        vec3 right = cross(normal, up);
        up = cross(normal, right);

        for(int i = 0; i < 6; i++)
        {
            coneDirection = normal;
            coneDirection += diffuseConeDirections[i].x * right + diffuseConeDirections[i].z * up;
            coneDirection = normalize(coneDirection);

            diffuseTrace += TraceCone(positionT, coneDirection, aperture, 1.0f, ambientOcclusion) * diffuseConeWeights[i];
        }

        diffuseTrace.rgb *= albedo;
    }

    vec4 result = (diffuseTrace + specularTrace) * bounceStrength;

    if(ambientOcclusion)
    {
        result.a = 1.0f - clamp(diffuseTrace.a, 0.0f, 1.0f); 
    }

    return clamp(result, 0.0f, 1.0f);
}

void main()
{
    // world-space position
    vec3 position = PositionFromDepth();
    // world-space normal
    vec3 normal = texture(gNormal, texCoord).xyz;
    // fragment albedo
    vec3 albedo = texture(gAlbedo, texCoord).rgb;
    // xyz = fragment specular, w = shininess
    vec4 specular = texture(gSpecular, texCoord);
    specular.a = specular.a * 1024.0f;
    // lighting cumulatives
    vec4 indirectLighting = CalculateIndirectLighting(position, normal, albedo, specular, enableAo == 0 ? false : true);
    // final color
    fragColor = indirectLighting;
}