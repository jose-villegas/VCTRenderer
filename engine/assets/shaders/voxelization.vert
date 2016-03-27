#version 430

out Vertex
{
	vec4 wsPosition;
    vec3 position;
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
	wsPosition = matrices.model * vec4(vertexPosition, 1.0f);

    position = wsPosition.xyz;
    normal = (matrices.normal * vec4(vertexNormal, 0.0f)).xyz;
    texCoord = vertexTexCoord;
    gl_Position = wsPosition;
}