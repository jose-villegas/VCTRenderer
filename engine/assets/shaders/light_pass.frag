#version 430

out vec4 fragColor;
in vec2 texCoord;

layout(binding = 0) uniform sampler2D gNormal;
layout(binding = 1) uniform sampler2D gAlbedo;
layout(binding = 2) uniform sampler2D gSpecular;
layout(binding = 3) uniform sampler2D gEmissive;
layout(binding = 4) uniform sampler2D gDepth;

layout(binding = 5) uniform sampler2D shadowMap;

layout(binding = 6, rgba8) uniform sampler3D voxelTex;
layout(binding = 7, rgba8) uniform sampler3D voxelTexMipmap[6];

const float PI = 3.14159265f;
const uint MAX_DIRECTIONAL_LIGHTS = 8;
const uint MAX_POINT_LIGHTS = 256;
const uint MAX_SPOT_LIGHTS = 256;

bool isEmissive = false;

struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};

struct Light {
    float angleInnerCone;
    float angleOuterCone;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;

    uint shadowingMethod;
    Attenuation attenuation;
};

uniform mat4 inverseProjectionView;
uniform mat4 lightViewProjection;

uniform vec3 cameraPosition;

uniform Light directionalLight[MAX_DIRECTIONAL_LIGHTS];
uniform Light pointLight[MAX_POINT_LIGHTS];
uniform Light spotLight[MAX_SPOT_LIGHTS];
uniform uint lightTypeCount[3];

uniform vec2 exponents;
uniform float lightBleedingReduction;

uniform float voxelScale;
uniform vec3 worldMinPoint;
uniform int volumeDimension;

uniform float maxTracingDistanceGlobal = 1.0f;
uniform float bounceStrength = 1.0f;
uniform float aoFalloff = 725.0f;
uniform float aoAlpha = 0.01f;
uniform uint mode = 0;

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
    vec3 weight = direction * direction;
    // navigation
    float voxelSize = 1.0f / volumeDimension;
    // move one voxel further to avoid self collision
    float dst = voxelSize * 2.0f;
    float diameter = aperture * dst;
    vec3 samplePos = direction * dst + position;
    // control vars
    float mipLevel = 0.0f;
    float anisoLevel = 0.0f;
    float mipMaxLevel = log2(volumeDimension) - 1.0f;
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
        anisoSample = weight.x * textureLod(voxelTexMipmap[visibleFace.x], samplePos, anisoLevel)
                    + weight.y * textureLod(voxelTexMipmap[visibleFace.y], samplePos, anisoLevel)
                    + weight.z * textureLod(voxelTexMipmap[visibleFace.z], samplePos, anisoLevel);

       if(mipLevel < 1.0f)
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
    }

    return vec4(coneSample.rgb, occlusion);
}

float TraceShadowCone(vec3 position, vec3 direction, float aperture, float maxTracingDistance) 
{
    if(isEmissive) { return 1.0f; }

    uvec3 visibleFace;
    visibleFace.x = (direction.x < 0.0) ? 0 : 1;
    visibleFace.y = (direction.y < 0.0) ? 2 : 3;
    visibleFace.z = (direction.z < 0.0) ? 4 : 5;
    // weight per axis for aniso sampling
    vec3 weight = direction * direction;
    // navigation
    float voxelSize = 1.0f / volumeDimension;
    // move one voxel further to avoid self collision
    float dst = voxelSize * 2.0f;
    float diameter = aperture * dst;
    vec3 samplePos = direction * dst + position;
    // control variables
    float visibility = 0.0f;
    float mipLevel = 0.0f;
    float anisoLevel = 0.0f;
    float mipMaxLevel = log2(volumeDimension) - 1.0f;
    // accumulated sample
    vec4 baseColor = vec4(0.0f);
    vec4 anisoSample = vec4(0.0f);

    while (visibility <= 1.0f && anisoSample.a != 1.0f && 
            dst <= maxTracingDistance && dst <= maxTracingDistanceGlobal) 
    {
        if (aperture < 0.3f && (samplePos.x < 0.0f || samplePos.y < 0.0f || samplePos.z < 0.0f
            || samplePos.x > 1.0f || samplePos.y > 1.0f || samplePos.z > 1.0f)) 
        { 
            break; 
        }

        mipLevel = clamp(log2(diameter * volumeDimension), 0.0f, mipMaxLevel);
        anisoLevel = max(mipLevel - 1.0f, 0.0f);
        // aniso sampling
        anisoSample = weight.x * textureLod(voxelTexMipmap[visibleFace.x], samplePos, anisoLevel)
                    + weight.y * textureLod(voxelTexMipmap[visibleFace.y], samplePos, anisoLevel)
                    + weight.z * textureLod(voxelTexMipmap[visibleFace.z], samplePos, anisoLevel);
        
        if(mipLevel < 1.0f)
        {
            baseColor = texture(voxelTex, samplePos);
            anisoSample = mix(baseColor, anisoSample, clamp(mipLevel, 0.0f, 1.0f));
        }
        // accumulate
        visibility += (1.0f - visibility) * anisoSample.a;
        // move further into volume
        dst += max(diameter, voxelSize);
        diameter = dst * aperture;
        samplePos = direction * dst + position;
    }

    return 1.0f - visibility;
}

float linstep(float low, float high, float value)
{
    return clamp((value - low) / (high - low), 0.0f, 1.0f);
}  

float ReduceLightBleeding(float pMax, float Amount)  
{  
    return linstep(Amount, 1, pMax);  
} 

vec2 WarpDepth(float depth)
{
    depth = 2.0f * depth - 1.0f;
    float pos = exp(exponents.x * depth);
    float neg = -exp(-exponents.y * depth);
    return vec2(pos, neg);
}

float Chebyshev(vec2 moments, float mean, float minVariance)
{
    if(mean <= moments.x)
    {
        return 1.0f;
    }
    else
    {
        float variance = moments.y - (moments.x * moments.x);
        variance = max(variance, minVariance);
        float d = mean - moments.x;
        float lit = variance / (variance + (d * d));
        return ReduceLightBleeding(lit, lightBleedingReduction);
    }
}

float Visibility(vec3 position)
{
    if(isEmissive) { return 1.0f; }

    vec4 lsPos = lightViewProjection * vec4(position, 1.0f);
    // avoid arithmetic error
    if(lsPos.w == 0.0f) return 1.0f;
    // transform to ndc-space
    lsPos /= lsPos.w;
    // querry visibility
    vec4 moments = texture(shadowMap, lsPos.xy).xyzw;
    // move to avoid acne
    vec2 wDepth = WarpDepth(lsPos.z - 0.0001f);
    // derivative of warping at depth
    vec2 depthScale = 0.0002f * exponents * wDepth;
    vec2 minVariance = depthScale * depthScale;
    // evsm mode 4 compares negative and positive
    float positive = Chebyshev(moments.xz, wDepth.x, minVariance.x);
    float negative = Chebyshev(moments.yw, wDepth.y, minVariance.y);
    // shadowing value
    return min(positive, negative);
}

vec3 Ambient(Light light, vec3 albedo)
{
    return max(albedo * light.ambient, 0.0f);
}

vec3 Diffuse(Light light, vec3 lightDirection, vec3 normal, vec3 albedo)
{
    float lambertian = max(dot(normal, lightDirection), 0.0f);
    return light.diffuse * albedo * lambertian;
}

vec3 Specular(Light light, vec3 lightDirection, vec3 normal, vec3 position, vec4 specular)
{
    vec3 viewDirection = normalize(cameraPosition - position);
    vec3 halfDirection = normalize(lightDirection + viewDirection);
    float specularFactor = max(dot(halfDirection, normal), 0.0f);
    specularFactor = pow(specularFactor, specular.a);
    return light.specular * specular.rgb * specularFactor;
}

vec3 CalculateDirectional(Light light, vec3 normal, vec3 position, vec3 albedo, vec4 specular)
{
    float visibility = 1.0f;

    if(light.shadowingMethod == 1)
    {
        visibility = Visibility(position);
    }
    else if(light.shadowingMethod == 2)
    {
        vec3 voxelPos = WorldToVoxelSample(position);
        visibility = max(0.0f, TraceShadowCone(voxelPos, light.direction, 0.01f, 1.0f));
    }

    if(visibility <= 0.0f) return vec3(0.0f);  

    return (Diffuse(light, light.direction, normal, albedo) 
           + Specular(light, light.direction, normal, position, specular)) * visibility;
}

vec3 CalculatePoint(Light light, vec3 normal, vec3 position, vec3 albedo, vec4 specular)
{
    light.direction = light.position - position;
    float d = length(light.direction);
    light.direction = normalize(light.direction);
    float falloff = 1.0f / (light.attenuation.constant + light.attenuation.linear * d
                    + light.attenuation.quadratic * d * d + 1.0f);

    if(falloff <= 0.0f) return vec3(0.0f);

    float visibility = 1.0f;

    if(light.shadowingMethod == 2)
    {
        vec3 voxelPos = WorldToVoxelSample(position);
        vec3 lightPosT = WorldToVoxelSample(light.position);

        vec3 lightDirT = lightPosT.xyz - voxelPos.xyz;
        float dT = length(lightDirT);
        lightDirT = normalize(lightDirT);

        visibility = max(0.0f, TraceShadowCone(voxelPos, lightDirT, 0.01f, dT));
    }    

    if(visibility <= 0.0f) return vec3(0.0f);  

    return (Diffuse(light, light.direction, normal, albedo) 
           + Specular(light, light.direction, normal, position, specular)) * falloff * visibility;
}

vec3 CalculateSpot(Light light, vec3 normal, vec3 position, vec3 albedo, vec4 specular)
{
    vec3 spotDirection = light.direction;
    light.direction = normalize(light.position - position);
    float cosAngle = dot(-light.direction, spotDirection);

    // outside the cone
    if(cosAngle <= light.angleOuterCone) { return vec3(0.0f); }

    // assuming they are passed as cos(angle)
    float innerMinusOuter = light.angleInnerCone - light.angleOuterCone;
    // spot light factor for smooth transition
    float spotMark = (cosAngle - light.angleOuterCone) / innerMinusOuter;
    float spotFalloff = smoothstep(0.0f, 1.0f, spotMark);

    if(spotFalloff <= 0.0f) return vec3(0.0f);   

    float dst = distance(light.position, position);
    float falloff = 1.0f / (light.attenuation.constant + light.attenuation.linear * dst
                    + light.attenuation.quadratic * dst * dst + 1.0f);   

    if(falloff <= 0.0f) return vec3(0.0f);

    float visibility = 1.0f;


    if(light.shadowingMethod == 2)
    {
        vec3 voxelPos = WorldToVoxelSample(position);
        vec3 lightPosT = WorldToVoxelSample(light.position);

        vec3 lightDirT = lightPosT.xyz - voxelPos.xyz;
        float dT = length(lightDirT);
        lightDirT = normalize(lightDirT);

        visibility = max(0.0f, TraceShadowCone(voxelPos, lightDirT, 0.01f, dT));
    }

    if(visibility <= 0.0f) return vec3(0.0f); 

    return (Diffuse(light, light.direction, normal, albedo) 
           + Specular(light, light.direction, normal, position, specular)) 
           * falloff * spotFalloff * visibility;
}

vec3 PositionFromDepth()
{
    float z = texture(gDepth, texCoord).x * 2.0f - 1.0f;
    vec4 projected = vec4(texCoord * 2.0f - 1.0f, z, 1.0f);
    projected = inverseProjectionView * projected;
    return projected.xyz / projected.w;
}

vec3 CalculateDirectLighting(vec3 position, vec3 normal, vec3 albedo, vec4 specular)
{
    // calculate directional lighting
    vec3 directLighting = vec3(0.0f);

    // calculate lighting for directional lights
    for(int i = 0; i < lightTypeCount[0]; ++i)
    {
        directLighting += CalculateDirectional(directionalLight[i], normal, position, 
                                         albedo, specular);
        directLighting += Ambient(directionalLight[i], albedo);
    }

    // calculate lighting for point lights
    for(int i = 0; i < lightTypeCount[1]; ++i)
    {
        directLighting += CalculatePoint(pointLight[i], normal, position, 
                                   albedo, specular);
        directLighting += Ambient(pointLight[i], albedo);
    }

    // calculate lighting for spot lights
    for(int i = 0; i < lightTypeCount[2]; ++i) 
    {
        directLighting += CalculateSpot(spotLight[i], normal, position, 
                                  albedo, specular);
        directLighting += Ambient(spotLight[i], albedo);
    }

    return directLighting;
}

vec4 CalculateIndirectLighting(vec3 position, vec3 normal, vec3 albedo, vec4 specular, bool ambientOcclusion)
{
    vec3 positionT = WorldToVoxelSample(position);
    vec3 cameraPosT = WorldToVoxelSample(cameraPosition);

    vec4 specularTrace = vec4(0.0f);
    vec4 diffuseTrace = vec4(0.0f);
    vec3 coneDirection = vec3(0.0f);

    // component greater than zero
    if(any(greaterThan(specular.rgb, specularTrace.rgb)))
    {
        vec3 viewDirection = normalize(cameraPosT - positionT);
        vec3 coneDirection = reflect(-viewDirection, normal);
        coneDirection = normalize(coneDirection);
        // specular cone setup
        float smoothness = pow(specular.a / 64.0f, 4.0f);
        float aperture = clamp(1.0f - sin(acos(0.11f / (smoothness + 0.11f))), 0.05f, 1.0f);
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

    if(ambientOcclusion) result.a = 1.0f - clamp(diffuseTrace.a, 0.0f, 1.0f);

    return clamp(result, 0.0f, 1.0f);
}

void main()
{
    // world-space position
    vec3 position = PositionFromDepth();
    // world-space normal
    vec3 normal = normalize(texture(gNormal, texCoord).xyz);
    // fragment albedo
    vec3 albedo = texture(gAlbedo, texCoord).rgb;
    // fragment emissiviness
    vec3 emissive = texture(gEmissive, texCoord).rgb;
    isEmissive = any(greaterThan(emissive, vec3(0.0f)));
    // xyz = fragment specular, w = shininess
    vec4 specular = texture(gSpecular, texCoord);
    specular.a *= 64.0f;
    // lighting cumulatives
    vec3 directLighting = vec3(1.0f);
    vec4 indirectLighting = vec4(1.0f);
    vec3 compositeLighting = vec3(1.0f);
    float ambientOcclusion = 1.0f;

    if(mode == 0)   // direct + indirect + ao
    {
        directLighting = CalculateDirectLighting(position, normal, albedo, specular);
        indirectLighting = CalculateIndirectLighting(position, normal, albedo, specular, true);
        ambientOcclusion = min(indirectLighting.a + aoAlpha, 1.0f);
    }
    else if(mode == 1)  // direct + indirect
    {
        directLighting = CalculateDirectLighting(position, normal, albedo, specular);
        indirectLighting = CalculateIndirectLighting(position, normal, albedo, specular, false);
    }
    else if(mode == 2) // direct only
    {
        directLighting = CalculateDirectLighting(position, normal, albedo, specular);
        indirectLighting = vec4(0.0f);
    }
    else if(mode == 3) // indirect only
    {
        directLighting = vec3(0.0f);
        albedo.rgb = albedo.rgb = vec3(1.0f);
        indirectLighting = CalculateIndirectLighting(position, normal, albedo, specular, false);
    }
    else if(mode == 4) // ambient occlusion only
    {
        directLighting = vec3(0.0f);
        specular = vec4(0.0f);
        indirectLighting = CalculateIndirectLighting(position, normal, albedo, specular, true);
        ambientOcclusion = min(indirectLighting.a + aoAlpha, 1.0f);
        indirectLighting = vec4(1.0f);
    }

    compositeLighting = (directLighting + indirectLighting.rgb) * ambientOcclusion;
    compositeLighting.rgb += emissive;
    // final color
    fragColor = vec4(clamp(compositeLighting, 0.0f, 1.0f), 1.0f);
}