#version 440

layout(location = 0) out vec4 fragColor;

in vec3 position;
in vec3 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec3 normalView;

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 transparent;
    float shininess;
    float refractionIndex;
} material;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D ambientMap;

void main()
{
    vec3 color = texture(ambientMap, texCoord.xy).rgb * material.ambient;
    color += texture(diffuseMap, texCoord.xy).rgb * material.diffuse;
    color += texture(specularMap, texCoord.xy).rgb * material.specular;
    vec3 lDir = normalize(vec3(0.0f, 1.0f, 0.0f) - position);
    vec3 iDiff = color * max(dot(normal, lDir), 0.0f);
    iDiff = clamp(iDiff, 0.0f, 1.0f);
    fragColor = vec4(color, 1.0f);
}