#version 420

out Vertex
{
    vec3 position;
    vec3 texCoord;
    vec3 normal;
} Out;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

uniform struct Matrices
{
    mat4 modelView;
    mat4 modelViewProjection;
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 normal;
} matrices;

void main()
{
    vec4 vertexPos = vec4(vertexPosition, 1.0);
    vec4 vertexNorm = vec4(vertexNormal, 0.0);
    Out.texCoord = vertexTexCoord;
    // space transform
    Out.position = (matrices.modelView * vertexPos).xyz;
    // not using normal matrix, since camera is at 0,0,0
    Out.normal = (matrices.modelView * vertexNorm).xyz;
}