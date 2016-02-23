#version 330

out vec4 fragColor;

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

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform vec2 screenSize;
uniform	Light directionalLight[MAX_DIRECTIONAL_LIGHTS];

void main()
{
    vec2 texCoord = gl_FragCoord.xy / screenSize;
    // retrieve gbuffer data
    vec3 position = texture(gPosition, texCoord).xyz;
    vec3 normal = texture(gNormal, texCoord).xyz;
    vec3 albedo = texture(gAlbedo, texCoord).rgb;
    vec3 specular = texture(gSpecular, texCoord).rgb;

    // calculate lighting for directional lights
    vec3 lighting = vec3(0.0f);
    vec3 viewDir = normalize(-position);

    for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
    {
        // add ambient component
        lighting += albedo * directionalLight[i].ambient;
        // get directional light direction
        vec3 lightDir = -normalize(directionalLight[i].direction);
        // diffuse color factor
        float diffuseFactor = dot(normal, -lightDir);
        // in a lighten fragment
        if(diffuseFactor > 0.0f)
        {
            // add diffuse component
            lighting += albedo * diffuseFactor * directionalLight[i].diffuse;

            // specular factor
            vec3 lightReflect = normalize(reflect(lightDir, normal));
            float specularFactor = dot(viewDir, lightReflect);

            // in specular lighting
            if(specularFactor > 0.0)
            {
                // add specular component
                lighting += specular * specularFactor * directionalLight[i].specular;
            }
        }

    }

    // final color
    fragColor = vec4(lighting, 1.0);
}