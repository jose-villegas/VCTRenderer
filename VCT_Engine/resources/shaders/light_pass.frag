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

uniform float ambientLight = 0.1;

uniform vec3 viewPosition;

void main()
{
    // retrieve gbuffer data
    vec3 fragPos = texture(gPosition, texCoord).rgb;
    vec3 normal = texture(gNormal, texCoord).rbg;
    vec3 albedo = texture(gAlbedo, texCoord).rgb;
    float specular = texture(gSpecular, texCoord).r;

    vec3 lightDir = vec3(0.5, -1.0, 0.0);
    // calculate lighting
    vec3 lighting = albedo * ambientLight; // ambient;

    float diffuseFactor = max(dot(normal, -lightDir), 0.0);
    vec3 viewDir = normalize(viewPosition - fragPos);
    vec3 lightReflect = normalize(reflect(lightDir, normal));
    float specularFactor = max(dot(viewDir, lightReflect), 1.0);

    lighting += albedo * diffuseFactor + specular * specularFactor;

    fragColor = vec4(lighting, 1.0);
}