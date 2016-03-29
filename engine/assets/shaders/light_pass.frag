#version 430

out vec4 fragColor;

in vec2 texCoord;

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

    Attenuation attenuation;
};

uniform mat4 inverseProjectionView;
uniform mat4 lightViewProjection;

uniform sampler2D gDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform sampler2DShadow shadowMap;
uniform bool traceShadows = false;

uniform sampler3D voxelVolume;

uniform	Light directionalLight[MAX_DIRECTIONAL_LIGHTS];
uniform Light pointLight[MAX_POINT_LIGHTS];
uniform Light spotLight[MAX_SPOT_LIGHTS];

uniform uint lightTypeCount[3];

float TraceCone(vec3 pos, vec3 dir, float aperture, float maxTracingDistance) 
{
  ivec3 voxelDimensions = textureSize(voxelVolume, 0);
  float voxelRes = float(voxelDimensions.x);
  vec3 weight = abs(dir);
  float voxelSize = 1.0 / voxelRes;
  float dst = voxelSize * 2;
  float diameter = aperture * dst;
  vec3 samplePos = dir * dst + pos;
  float shadowSample = 0.0;

  while (shadowSample <= 1.0 && dst <= maxTracingDistance) {

    if (aperture < 0.3 && (samplePos.x < 0 || samplePos.y < 0 || samplePos.z < 0
                       || samplePos.x > 1 || samplePos.y > 1 || samplePos.z > 1)) {

      break;
    }

    float mipLevel = max(log2(diameter * voxelRes), 0);

    vec4 interpolatedSample = weight.x * textureLod(voxelVolume, samplePos, mipLevel)
                            + weight.y * textureLod(voxelVolume, samplePos, mipLevel)
                            + weight.z * textureLod(voxelVolume, samplePos, mipLevel);

    shadowSample += (1 - shadowSample) * interpolatedSample.a;
    dst += max(diameter, voxelSize);
    diameter = dst * aperture;
    samplePos = dir * dst + pos;
  }

  return shadowSample;

  return 1.0f;
}

float Visibility(vec3 position)
{
    if(traceShadows)
    {
        return 1.0f;
    }
    else
    {
        vec4 lsPos = lightViewProjection * vec4(position, 1.0f);
        // transform to ndc-space
        lsPos /= lsPos.w;
        // querry visibility
        return texture(shadowMap, lsPos.xyz);
    }
}

vec3 Ambient(Light light, vec3 albedo)
{
    return albedo * light.ambient;
}

vec3 Diffuse(Light light, vec3 lightDirection, vec3 normal, vec3 albedo)
{
    float lambertian = max(dot(normal, lightDirection), 0.0f);
    return light.diffuse * albedo * lambertian;
}

vec3 Specular(Light light, vec3 lightDirection, vec3 normal, vec3 position, vec4 specular)
{
    vec3 halfDirection = normalize(lightDirection + normalize(-position));
    float specularFactor = max(dot(halfDirection, normal), 0.0f);
    specularFactor = pow(specularFactor, specular.a * 128.0f);

    return light.specular * specular.rgb * specularFactor;
}

vec3 CalculateDirectional(Light light, vec3 normal, vec3 position, vec3 albedo, vec4 specular)
{
    return Diffuse(light, light.direction, normal, albedo) 
           + Specular(light, light.direction, normal, position, specular);
}

vec3 CalculatePoint(Light light, vec3 normal, vec3 position, vec3 albedo, vec4 specular)
{
    light.direction = normalize(light.position - position);
    float distance = distance(light.position, position);
    float falloff = 1.0f / (light.attenuation.constant + light.attenuation.linear * distance
                    + light.attenuation.quadratic * distance * distance + 1.0f);

    if(falloff <= 0.0f) { return Ambient(light, albedo); }             

    return (Diffuse(light, light.direction, normal, albedo) 
           + Specular(light, light.direction, normal, position, specular)) * falloff;
}

vec3 CalculateSpot(Light light, vec3 normal, vec3 position, vec3 albedo, vec4 specular)
{
    vec3 spotDirection = light.direction;
    light.direction = normalize(light.position - position);
    float cosAngle = dot(-light.direction, spotDirection);

    if(cosAngle <= light.angleOuterCone) { return Ambient(light, albedo); }

    // assuming they are passed as cos(angle)
    float innerMinusOuter = light.angleInnerCone - light.angleOuterCone;
    // spot light factor for smooth transition
    float spotMark = (cosAngle - light.angleOuterCone) / innerMinusOuter;
    float spotFalloff = smoothstep(0.0f, 1.0f, spotMark);

    float dst = distance(light.position, position);
    float falloff = 1.0f / (light.attenuation.constant + light.attenuation.linear * dst
                    + light.attenuation.quadratic * dst * dst + 1.0f);

    if(falloff <= 0.0f) { return Ambient(light, albedo); }             

    return (Diffuse(light, light.direction, normal, albedo) 
           + Specular(light, light.direction, normal, position, specular)) 
           * falloff * spotFalloff;
}

vec3 PositionFromDepth()
{
    float z = texture(gDepth, texCoord).x * 2.0f - 1.0f;
    vec4 projected = vec4(texCoord * 2.0f - 1.0f, z, 1.0f);
    projected = inverseProjectionView * projected;
    return projected.xyz / projected.w;
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

    vec3 lighting = vec3(0.0f);

    // calculate lighting for directional lights
    for(int i = 0; i < lightTypeCount[0]; ++i)
    {
        lighting += CalculateDirectional(directionalLight[i], normal, position, 
                                         albedo, specular);
        // only directionals casts shadows (in this app), thus we can check here
        if(i == 0) { lighting *= Visibility(position); }

        lighting += Ambient(directionalLight[i], albedo);
    }

    // calculate lighting for point lights
    for(int i = 0; i < lightTypeCount[1]; ++i)
    {
        lighting += CalculatePoint(pointLight[i], normal, position, 
                                   albedo, specular);
        lighting += Ambient(pointLight[i], albedo);
    }

    // calculate lighting for spot lights
    for(int i = 0; i < lightTypeCount[2]; ++i) 
    {
        lighting += CalculateSpot(spotLight[i], normal, position, 
                                  albedo, specular);
        lighting += Ambient(spotLight[i], albedo);
    }

    // final color
    fragColor = vec4(lighting, 1.0);
}