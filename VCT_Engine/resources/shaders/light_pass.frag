#version 440

const uint N_LIGHTS = 256;

out vec4 fragColor;

in vec2 texCoord;

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

    uint lightType;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform float ambientFactor = 0.01;

uniform vec3 viewPosition;

// ubo with all the scene available number of lights
layout (std140) uniform AvailableLights {
    Light light[N_LIGHTS];
};

void main()
{
    // retrieve gbuffer data
    vec3 fragPos = texture(gPosition, texCoord).rgb;
    vec3 normal = texture(gNormal, texCoord).rbg;
    vec3 albedo = texture(gAlbedo, texCoord).rgb;
    float specular = texture(gSpecular, texCoord).r;

    vec3 lightDir = vec3(0.5, -1.0, 0.0);
    // calculate lighting
    vec3 lighting = albedo * ambientFactor; // ambient;
    // diffuse color factor
    float diffuseFactor = max(dot(normal, -lightDir), 0.0);
    // specular color factor
    vec3 viewDir = normalize(viewPosition - fragPos);
    vec3 lightReflect = normalize(reflect(lightDir, normal));
    float specularFactor = max(dot(viewDir, lightReflect), 1.0);
    // ambient + diffuse + specular
    lighting += albedo * diffuseFactor;
    lighting += specular * specularFactor;

    for(int i = 0; i < N_LIGHTS; i++)
    {
        vec3 liDir = normalize(light[i].position - fragPos);
        vec3 liDif = max(dot(normal, liDir), 0.0) * albedo * light[i].diffuse;
        lighting += liDif;
    }
    // final color
    fragColor = vec4(lighting, 1.0);
}