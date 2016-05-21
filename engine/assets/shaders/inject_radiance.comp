#version 430
#extension GL_ARB_shader_image_load_store : require
layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout(binding = 0) uniform sampler3D voxelAlbedo;
layout(binding = 1, rgba8) uniform image3D voxelNormal;
layout(binding = 2, rgba8) uniform image3D writeonly voxelRadiance;
layout(binding = 3, rgba8) uniform image3D readonly voxelEmissive;

const float PI = 3.14159265f;
const float EPSILON = 1e-30;
const float SQRT_3 = 1.73205080f;
const uint MAX_DIRECTIONAL_LIGHTS = 3;
const uint MAX_POINT_LIGHTS = 6;
const uint MAX_SPOT_LIGHTS = 6;

struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};

struct Light {
    float angleInnerCone;
    float angleOuterCone;
    vec3 diffuse;
    vec3 position;
    vec3 direction;
    uint shadowingMethod;
    Attenuation attenuation;
};

uniform Light directionalLight[MAX_DIRECTIONAL_LIGHTS];
uniform Light pointLight[MAX_POINT_LIGHTS];
uniform Light spotLight[MAX_SPOT_LIGHTS];
uniform uint lightTypeCount[3];

uniform vec2 exponents;
uniform float lightBleedingReduction;
uniform mat4 lightViewProjection;
uniform float voxelSize;
uniform float voxelScale;
uniform vec3 worldMinPoint;

uniform sampler2D shadowMap;
uniform int volumeDimension;
uniform uint storeVisibility;
uniform uint normalWeightedLambert = 1;

uniform float traceShadowHit;

vec3 VoxelToWorld(ivec3 pos)
{
    vec3 result = vec3(pos);
    result *= voxelSize;

    return result + worldMinPoint;
}

vec3 WorldToVoxel(vec3 position)
{
    vec3 voxelPos = position - worldMinPoint;
    return voxelPos * voxelScale;
}

float TraceShadow(vec3 position, vec3 direction, float maxTracingDistance) 
{
    // scaling factor
    float k = traceShadowHit * traceShadowHit;
    // navigation
    float voxelTexSize = 1.0f / volumeDimension;
    // move one voxel further to avoid self collision
    float dst = voxelTexSize * 2.0f;
    vec3 samplePos = direction * dst + position;
    // control variables
    float visibility = 0.0f;
    // accumulated sample
    float traceSample = 0.0f;

    while (visibility <= 1.0f && dst <= maxTracingDistance) 
    {
        if (samplePos.x < 0.0f || samplePos.y < 0.0f || samplePos.z < 0.0f
            || samplePos.x > 1.0f || samplePos.y > 1.0f || samplePos.z > 1.0f) 
        { 
            break; 
        }
        
        traceSample = ceil(texture(voxelAlbedo, samplePos).a) * k;

        // hard shadows mode
        if(traceSample > 1.0f - EPSILON) { return 0.0f; }

        // accumulate
        visibility += (1.0f - visibility) * traceSample / dst;
        // move further into volume
        dst += voxelTexSize;
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

vec3 BRDF(Light light, vec3 normal, vec3 albedo)
{
    float nDotL = 0.0f;

    if(normalWeightedLambert == 1)
    {
        vec3 weight = normal * normal;
        // calculate directional normal attenuation
        float rDotL = dot(vec3(1.0, 0.0, 0.0), light.direction);
        float uDotL = dot(vec3(0.0, 1.0, 0.0), light.direction);
        float fDotL = dot(vec3(0.0, 0.0, 1.0), light.direction);

        rDotL = normal.x > 0.0 ? max(rDotL, 0.0) : max(-rDotL, 0.0);
        uDotL = normal.y > 0.0 ? max(uDotL, 0.0) : max(-uDotL, 0.0);
        fDotL = normal.z > 0.0 ? max(fDotL, 0.0) : max(-fDotL, 0.0);
        // voxel shading average from all front sides
        nDotL = rDotL * weight.x + uDotL * weight.y + fDotL * weight.z;
    }
    else
    {
        nDotL = max(dot(normal, light.direction), 0.0f);
    }

    return light.diffuse * albedo * nDotL;
}

vec4 CalculateDirectional(Light light, vec3 normal, vec3 position, vec3 albedo)
{
    float visibility = 1.0f;

    if(light.shadowingMethod == 1)
    {
        visibility = Visibility(position);
    }
    else if(light.shadowingMethod == 2)
    {
        vec3 voxelPos = WorldToVoxel(position);
        visibility = TraceShadow(voxelPos, light.direction, 1.0f);
    }

    if(visibility == 0.0f) return vec4(0.0f); 

    return vec4(BRDF(light, normal, albedo) * visibility, visibility);
}

vec4 CalculatePoint(Light light, vec3 normal, vec3 position, vec3 albedo)
{
    light.direction = light.position - position;
    float d = length(light.direction);
    light.direction = normalize(light.direction);
    float falloff = 1.0f / (light.attenuation.constant + light.attenuation.linear * d
                    + light.attenuation.quadratic * d * d + 1.0f);

    if(falloff <= 0.0f) return vec4(0.0f);

    float visibility = 1.0f;

    if(light.shadowingMethod == 2)
    {
        vec3 voxelPos = WorldToVoxel(position);
        vec3 lightPosT = WorldToVoxel(light.position);

        vec3 lightDirT = lightPosT.xyz - voxelPos.xyz;
        float dT = length(lightDirT);
        lightDirT = normalize(lightDirT);

        visibility = TraceShadow(voxelPos, lightDirT, dT);
    }    

    if(visibility <= 0.0f) return vec4(0.0f); 

    return vec4(BRDF(light, normal, albedo) * falloff * visibility, visibility);
}

vec4 CalculateSpot(Light light, vec3 normal, vec3 position, vec3 albedo)
{
    vec3 spotDirection = light.direction;
    light.direction = normalize(light.position - position);
    float cosAngle = dot(-light.direction, spotDirection);

    // outside the cone
    if(cosAngle < light.angleOuterCone) { return vec4(0.0f); }

    // assuming they are passed as cos(angle)
    float innerMinusOuter = light.angleInnerCone - light.angleOuterCone;
    // spot light factor for smooth transition
    float spotMark = (cosAngle - light.angleOuterCone) / innerMinusOuter;
    float spotFalloff = smoothstep(0.0f, 1.0f, spotMark);

    if(spotFalloff <= 0.0f) return vec4(0.0f);   

    float dst = distance(light.position, position);
    float falloff = 1.0f / (light.attenuation.constant + light.attenuation.linear * dst
                    + light.attenuation.quadratic * dst * dst + 1.0f);   

    if(falloff <= 0.0f) return vec4(0.0f);

    float visibility = 1.0f;

    if(light.shadowingMethod == 2)
    {
        vec3 voxelPos = WorldToVoxel(position);
        vec3 lightPosT = WorldToVoxel(light.position);

        vec3 lightDirT = lightPosT.xyz - voxelPos.xyz;
        float dT = length(lightDirT);
        lightDirT = normalize(lightDirT);

        visibility = TraceShadow(voxelPos, lightDirT, dT);
    }    

    if(visibility <= 0.0f) return vec4(0.0f);

    return vec4(BRDF(light, normal, albedo) * falloff * spotFalloff * visibility, visibility);
}

vec4 CalculateDirectLighting(vec3 position, vec3 normal, vec3 albedo)
{
    normal = normalize(normal);
    // world space grid voxel size
    float voxelWorldSize = 1.0 /  (voxelScale * volumeDimension);
    // calculate directional lighting
    vec4 directLighting = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 current =  vec4(0.0f); int count = 0;
	// move position forward to avoid shadowing errors
    position = position + normal * voxelWorldSize * 0.5f;

    // calculate lighting for directional lights
    for(int i = 0; i < lightTypeCount[0]; ++i)
    {
        current = CalculateDirectional(directionalLight[i], normal, position, albedo);
        directLighting.rgb += current.rgb;
        directLighting.a += current.a; count++;
    }

    // calculate lighting for point lights
    for(int i = 0; i < lightTypeCount[1]; ++i)
    {
        current = CalculatePoint(pointLight[i], normal, position, albedo);
        directLighting.rgb += current.rgb;
        directLighting.a += current.a; count++;
    }

    // calculate lighting for spot lights
    for(int i = 0; i < lightTypeCount[2]; ++i) 
    {
        current = CalculateSpot(spotLight[i], normal, position, albedo);
        directLighting.rgb += current.rgb;
        directLighting.a += current.a; count++;
    }

    if(count > 0) { directLighting.a /= count; }

    return directLighting;
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
    // voxel color
	vec4 albedo = texelFetch(voxelAlbedo, writePos, 0);

    if(albedo.a < EPSILON) { return; }

    albedo.a = 0.0f;
    // voxel normal in 0-1 range
    vec3 baseNormal = imageLoad(voxelNormal, writePos).xyz;
    // normal is stored in 0-1 range, restore to -1-1
    vec3 normal = DecodeNormal(baseNormal);
    // emission from voxel
    vec3 emissive = imageLoad(voxelEmissive, writePos).rgb;

    // black voxel has no irradiance diffuse
    if(any(greaterThan(albedo.rgb, vec3(0.0f))))
    {
        // obtain world-space position of the current voxel
        vec3 wsPosition = VoxelToWorld(writePos);
        // calculate direct lighting onto voxel
        albedo = CalculateDirectLighting(wsPosition, normal, albedo.rgb);
    }

    // store visibility in normal alpha
    imageStore(voxelNormal, writePos, vec4(baseNormal, albedo.a));
    // add emission
    albedo.rgb += emissive;
    albedo.a = 1.0f;

	imageStore(voxelRadiance, writePos, albedo);
}