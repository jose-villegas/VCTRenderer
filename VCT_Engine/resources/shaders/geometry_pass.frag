#version 440

in vec3 position;
in vec3 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;

out vec4 fragColor;

void main()
{
    fragColor = vec4(normalize(normal), 1.0f);
}