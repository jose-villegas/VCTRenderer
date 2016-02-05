#version 330

out vec4 fragColor;

in vec2 texCoord;

struct Light {
    float angleInnerCone;
    float angleOuterCone;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform float ambientFactor = 0.01;

uniform vec3 viewPosition;

const uint LIGHT_MAX_DIRECTIONAL = 1;

// light uniforms
layout (std140) uniform SceneLights 
{
	Light directionalLight[LIGHT_MAX_DIRECTIONAL];
};

void main()
{
    // retrieve gbuffer data
    vec3 fragPos = texture(gPosition, texCoord).rgb;
    vec3 normal = texture(gNormal, texCoord).rbg;
    vec3 albedo = texture(gAlbedo, texCoord).rgb;
    float specular = texture(gSpecular, texCoord).r;

    // calculate lighting
    vec3 lighting = albedo * ambientFactor; // ambient;
    // specular color factor
    vec3 viewDir = normalize(viewPosition - fragPos);

    for(int i = 0; i < 1; i++)
    {
        vec3 lightDir = normalize(directionalLight[i].direction);
        // diffuse color factor
        float diffuseFactor = max(dot(normal, -lightDir), 0.0);
        vec3 lightReflect = normalize(reflect(lightDir, normal));
        float specularFactor = max(dot(viewDir, lightReflect), 1.0);

        // ambient + diffuse + specular
        lighting += albedo * diffuseFactor;
        lighting += specular * specularFactor;
    }
    // final color
    fragColor = vec4(lighting, 1.0);
}