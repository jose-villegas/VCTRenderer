#version 430

out Vertex
{
    vec3 texCoord;
    vec3 normal;
};

uniform struct Matrices
{
    mat4 model;
    mat4 normal;
} matrices;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;

void main()
{
    gl_Position = matrices.model * vec4(vertexPosition, 1.0f);

    normal = (matrices.normal * vec4(vertexNormal, 0.0f)).xyz;
    texCoord = vertexTexCoord;
}