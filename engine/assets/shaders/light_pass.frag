#version 430

layout(location = 0) out vec4 fragColor;

in vec2 texCoord;

layout(binding = 0) uniform sampler2D gNormal;
layout(binding = 1) uniform sampler2D gAlbedo;
layout(binding = 2) uniform sampler2D gSpecular;
layout(binding = 3) uniform sampler2D gEmissive;
layout(binding = 4) uniform sampler2D gDepth;

layout(binding = 5) uniform sampler2D shadowMap;
layout(binding = 6, rgba8) uniform sampler3D voxelVisibility;

layout(binding = 7, rgba8) uniform sampler3D voxelTex;
layout(binding = 8, rgba8) uniform sampler3D voxelTexMipmap[6];

const float PI = 3.14159265f;
const float EPSILON = 1e-30;
const uint MAX_DIRECTIONAL_LIGHTS = 8;
const uint MAX_POINT_LIGHTS = 256;
const uint MAX_SPOT_LIGHTS = 256;

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

vec3 WorldToVoxel(vec3 position)
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
    traceOcclusion = traceOcclusion && aoAlpha <= 1.0f;
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
        if (traceOcclusion && occlusion <= 1.0f) 
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

float TraceShadowCone(vec3 position, vec3 direction, float maxTracingDistance) 
{
    // navigation
    float voxelSize = 1.0f / volumeDimension;
    // move one voxel further to avoid self collision
    float dst = voxelSize * 2.0f;
    vec3 samplePos = direction * dst + position;
    // control variables
    float visibility = 0.0f;
    // accumulated sample
    vec4 traceSample = vec4(0.0f);

    while (visibility <= 1.0f && traceSample.a != 1.0f && 
            dst <= maxTracingDistance && dst <= maxTracingDistanceGlobal) 
    {
        if (samplePos.x < 0.0f || samplePos.y < 0.0f || samplePos.z < 0.0f
            || samplePos.x > 1.0f || samplePos.y > 1.0f || samplePos.z > 1.0f) 
        { 
            break; 
        }
        
        traceSample = texture(voxelTex, samplePos);
        // accumulate
        visibility += (1.0f - visibility) * traceSample.a;
        // move further into volume
        dst += voxelSize;
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
    vec4 lsPos = lightViewProjection * vec4(position, 1.0f);
    // avoid arithmetic error
    if(lsPos.w == 0.0f) return 1.0f;
    // transform to ndc-space
    lsPos /= lsPos.w;
    // querry visibility
    vec4 moments = texture(shadowMap, lsPos.xy);
    // move to avoid acne
    vec2 wDepth = WarpDepth(lsPos.z - 0.0001f);
    // derivative of warping at depth
    vec2 depthScale = 0.0001f * exponents * wDepth;
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
    float specularFactor = max(dot(normal, halfDirection), 0.0f);
    specularFactor = pow(specularFactor, specular.a * 256.0f);
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
        vec3 voxelPos = WorldToVoxel(position);
        visibility = max(0.0f, TraceShadowCone(voxelPos, light.direction, 1.0f));
    }
    else if(light.shadowingMethod == 3)
    {
        vec3 voxelPos = WorldToVoxel(position);  
        visibility = max(0.0f, texture(voxelVisibility, voxelPos).a);
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
        vec3 voxelPos = WorldToVoxel(position);
        vec3 lightPosT = WorldToVoxel(light.position);

        vec3 lightDirT = lightPosT.xyz - voxelPos.xyz;
        float dT = length(lightDirT);
        lightDirT = normalize(lightDirT);

        visibility = max(0.0f, TraceShadowCone(voxelPos, lightDirT, dT));
    }
    else if(light.shadowingMethod == 3)
    {
        vec3 voxelPos = WorldToVoxel(position);  
        visibility = max(0.0f, texture(voxelVisibility, voxelPos).a);
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
        vec3 voxelPos = WorldToVoxel(position);
        vec3 lightPosT = WorldToVoxel(light.position);

        vec3 lightDirT = lightPosT.xyz - voxelPos.xyz;
        float dT = length(lightDirT);
        lightDirT = normalize(lightDirT);

        visibility = max(0.0f, TraceShadowCone(voxelPos, lightDirT, dT));
    }
    else if(light.shadowingMethod == 3)
    {
        vec3 voxelPos = WorldToVoxel(position);  
        visibility = max(0.0f, texture(voxelVisibility, voxelPos).a);
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
    vec3 positionT = WorldToVoxel(position);
    vec3 cameraPosT = WorldToVoxel(cameraPosition);

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
        float aperture = 1.0f - sin(acos(0.11f / (specular.a * specular.a + 0.11f))) * 0.96f;
        aperture = clamp(aperture, 0.04f, 1.0f); // extremely thin cones slow down performance
        specularTrace = TraceCone(positionT.xyz, coneDirection, aperture, 1.0f, false);
        specularTrace.rgb *= specular.rgb;
    }

    // component greater than zero
    if(any(greaterThan(albedo, diffuseTrace.rgb)))
    {
        // diffuse cone setup
        float aperture = 0.4487f;
        vec3 guide = vec3(0.0f, 1.0f, 0.0f);

        if (abs(dot(normal,guide)) == 1.0f)
        {
            guide = vec3(0.0f, 0.0f, 1.0f);
        }

        // Find a tangent and a bitangent
        vec3 right = normalize(guide - dot(normal, guide) * normal);
        vec3 up = cross(right, normal);

        for(int i = 0; i < 6; i++)
        {
            coneDirection = normal;
            coneDirection += diffuseConeDirections[i].x * right + diffuseConeDirections[i].z * up;
            coneDirection = normalize(coneDirection);
            // cumulative result
            diffuseTrace += TraceCone(positionT, coneDirection, aperture, 1.0f, ambientOcclusion) * diffuseConeWeights[i];
        }

        diffuseTrace.rgb *= albedo;
    }

    vec3 result = (diffuseTrace.rgb + specularTrace.rgb) * bounceStrength;

    return vec4(result, ambientOcclusion ? clamp(1.0f - diffuseTrace.a, 0.0f, 1.0f) : 1.0f);
}

void main()
{
    // world-space position
    vec3 position = PositionFromDepth();
    // world-space normal
    vec3 normal = normalize(texture(gNormal, texCoord).xyz);
    // fragment albedo
    vec3 baseColor = texture(gAlbedo, texCoord).rgb;
    // convert to linear space
    vec3 albedo = pow(baseColor, vec3(2.2f));
    // fragment emissiviness
    vec3 emissive = texture(gEmissive, texCoord).rgb;
    // xyz = fragment specular, w = shininess
    vec4 specular = texture(gSpecular, texCoord);
    // lighting cumulatives
    vec3 directLighting = vec3(1.0f);
    vec4 indirectLighting = vec4(1.0f);
    vec3 compositeLighting = vec3(1.0f);

    if(mode == 0)   // direct + indirect + ao
    {
        directLighting = CalculateDirectLighting(position, normal, albedo, specular);
        indirectLighting = CalculateIndirectLighting(position, normal, baseColor, specular, true);
    }
    else if(mode == 1)  // direct + indirect
    {
        directLighting = CalculateDirectLighting(position, normal, albedo, specular);
        indirectLighting = CalculateIndirectLighting(position, normal, baseColor, specular, false);
    }
    else if(mode == 2) // direct only
    {
        directLighting = CalculateDirectLighting(position, normal, albedo, specular);
        indirectLighting = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    else if(mode == 3) // indirect only
    {
        directLighting = vec3(0.0f);
        albedo.rgb = albedo.rgb = vec3(1.0f);
        indirectLighting = CalculateIndirectLighting(position, normal, baseColor, specular, false);
    }
    else if(mode == 4) // ambient occlusion only
    {
        directLighting = vec3(0.0f);
        specular = vec4(0.0f);
        indirectLighting = CalculateIndirectLighting(position, normal, baseColor, specular, true);
        indirectLighting.rgb = vec3(1.0f);
    }

    // convert indirect to linear space
    indirectLighting.rgb = pow(indirectLighting.rgb, vec3(2.2f));
    // final composite lighting (direct + indirect) * ambient occlusion
    compositeLighting = (directLighting + indirectLighting.rgb) * indirectLighting.a;
    compositeLighting += emissive;

    // -- this could be done in a post-process pass -- 

    // Reinhard tone mapping
    compositeLighting = compositeLighting / (compositeLighting + 1.0f);
    // gamma correction
    const float gamma = 2.2;
    // convert to gamma space
    compositeLighting = pow(compositeLighting, vec3(1.0 / gamma));

    fragColor = vec4(compositeLighting, 1.0f);
}