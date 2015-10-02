#version 440

out vec3 position;
out vec3 texCoord;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;
out vec3 normalView;

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
    vec4 vertexPos = vec4(vertexPosition, 1.0f);
    vec4 vertexNorm = vec4(vertexNormal, 0.0f);
    texCoord = vertexTexCoord;
    // space transform
    normal = (matrices.normal * vertexNorm).xyz;
    position = (matrices.modelView * vertexPos).xyz;
    // model to camera view
    normalView = (matrices.modelView * vertexNorm).xyz;
    tangent = (matrices.modelView * vec4(vertexTangent, 0.0f)).xyz;
    bitangent = (matrices.modelView * vec4(vertexBitangent, 0.0f)).xyz;
    // final drawing pos
    gl_Position = matrices.modelViewProjection * vertexPos;
}