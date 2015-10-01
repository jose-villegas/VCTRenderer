#version 440

out vec4 fragColor;

in vec3 position;
in vec3 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec3 normalView;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D ambientMap;

void main()
{
    vec3 lDir = normalize(vec3(0.0f, 1.0f, 0.0f) - position);
    vec3 iDiff = vec3(0.7f) * max(dot(normal, lDir), 0.0f);
    iDiff = clamp(iDiff, 0.0f, 1.0f);
    fragColor = vec4(iDiff, 1.0f);
}