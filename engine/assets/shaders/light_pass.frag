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

uniform mat4 inverseProjection;

uniform sampler2D gDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform	Light directionalLight[MAX_DIRECTIONAL_LIGHTS];
uniform Light pointLight[MAX_POINT_LIGHTS];
uniform Light spotLight[MAX_SPOT_LIGHTS];

uniform uint lightTypeCount[3];

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
    return Ambient(light, albedo) + 
           Diffuse(light, light.direction, normal, albedo) + 
           Specular(light, light.direction, normal, position, specular);
}

vec3 CalculatePoint(Light light, vec3 normal, vec3 position, vec3 albedo, vec4 specular)
{
    light.direction = normalize(light.position - position);
    float distance = distance(light.position, position);
    float falloff = 1.0f / (light.attenuation.constant + light.attenuation.linear * distance
                    + light.attenuation.quadratic * distance * distance + 1.0f);

    if(falloff <= 0.0f) { return Ambient(light, albedo); }             

    return CalculateDirectional(light, normal, position, albedo, specular) * falloff;
}

vec3 CalculateSpot(Light light, vec3 normal, vec3 position, vec3 albedo, vec4 specular)
{
    vec3 spotDirection = light.direction;
    vec3 lightDirection = normalize(light.position - position);
    float cosAngle = dot(-lightDirection, spotDirection);

    if(cosAngle <= light.angleOuterCone) { return Ambient(light, albedo); }

    // assuming they are passed as cos(angle)
    float innerMinusOuter = light.angleInnerCone - light.angleOuterCone;
    // spot light factor for smooth transition
    float spotFalloff = smoothstep(0.0f, 1.0f, (cosAngle - light.angleOuterCone) / innerMinusOuter);
    // final result
    return CalculatePoint(light, normal, position, albedo, specular) * spotFalloff;
}

vec3 PositionFromDepth()
{
    float z = texture(gDepth, texCoord).x * 2.0f - 1.0f;
    vec4 projected = vec4(texCoord * 2.0f - 1.0f, z, 1.0f);
    projected = inverseProjection * projected;
    return projected.xyz / projected.w;
}

void main()
{
    // retrieve gbuffer data
    vec3 position = PositionFromDepth();
    vec3 normal = texture(gNormal, texCoord).xyz;
    vec3 albedo = texture(gAlbedo, texCoord).rgb;
    vec4 specular = texture(gSpecular, texCoord);

    // calculate lighting for directional lights
    vec3 lighting = vec3(0.0f);

    for(int i = 0; i < lightTypeCount[0]; ++i)
    {
        lighting += CalculateDirectional(directionalLight[i], normal, position, 
                                         albedo, specular);
    }

    // calculate lighting for point lights
    for(int i = 0; i < lightTypeCount[1]; ++i)
    {
        lighting += CalculatePoint(pointLight[i], normal, position, 
                                   albedo, specular);
    }

    // calculate lighting for spot lights
    for(int i = 0; i < lightTypeCount[2]; ++i) 
    {
        lighting += CalculateSpot(spotLight[i], normal, position, 
                                  albedo, specular);
    }

    // final color
    fragColor = vec4(lighting, 1.0);
}