#version 430

out Vertex
{
    vec3 position;
    vec3 texCoord;
    vec3 normal;
};

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;

void main()
{
    position = vertexPosition;
    normal = vertexNormal;
    texCoord = vertexTexCoord;
    gl_Position = vec4(position, 1.0f);
}