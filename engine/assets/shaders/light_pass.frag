#version 330

out vec4 fragColor;

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
uniform vec3 viewPosition;
uniform	Light directionalLight;

uniform float ambientFactor = 0.01;


void main()
{
    vec2 texCoord = gl_FragCoord.xy / screenSize;
    // retrieve gbuffer data
    vec3 position = texture(gPosition, texCoord).xyz;
    vec3 normal = texture(gNormal, texCoord).xyz;
    vec3 albedo = texture(gAlbedo, texCoord).rgb;
    vec3 specular = texture(gSpecular, texCoord).rgb;

    // calculate lighting
    vec3 lighting = albedo * ambientFactor; // ambient;
    // specular color factor
    vec3 viewDir = normalize(viewPosition - position);
    vec3 lightDir = -normalize(directionalLight.direction);

    for(int i = 0; i < 1; i++)
    {
        // diffuse color factor
        float diffuseFactor = dot(normal, -lightDir);

        if(diffuseFactor > 0.0f)
        {
            lighting += albedo * diffuseFactor;

            vec3 lightReflect = normalize(reflect(lightDir, normal));
            float specularFactor = dot(viewDir, lightReflect);

            if(specularFactor > 0.0)
            {
                lighting += specular * specularFactor;
            }
        }
    }
    // final color
    fragColor = vec4(lighting, 1.0);
}