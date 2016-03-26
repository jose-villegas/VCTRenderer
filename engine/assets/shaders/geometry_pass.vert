#version 430

out Vertex
{
    vec3 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
};

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

uniform struct Matrices
{
    mat4 normal;
    mat4 modelViewProjection;
} matrices;

void main()
{
    vec4 vertexPos = vec4(vertexPosition, 1.0);
    vec4 vertexNorm = vec4(vertexNormal, 0.0);
    texCoord = vertexTexCoord;
    // not using normal matrix, since camera is at 0,0,0
    normal = (matrices.normal * vertexNorm).xyz;
    // TBN matrix build - model to camera view
    tangent = (matrices.normal * vec4(vertexTangent, 0.0)).xyz;
    bitangent = (matrices.normal * vec4(vertexBitangent, 0.0)).xyz;
    // final drawing pos
    gl_Position = matrices.modelViewProjection * vertexPos;
}