#version 440

layout(std140) uniform Matrices
{
    mat4 modelView;
    mat4 modelViewProjection;
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 normal;
} matrix;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out vec3 position;
out vec3 normal;
out vec2 texCoord;
out vec3 tangent;
out vec3 bitangent;

void main(void)
{
    vec4 vertexPos = vec4(vertexPosition, 1.0f);
    // space transform
    normal = normalize(matrix.normal * vec4(vertexNormal, 0.0f)).xyz;
    position = (matrix.modelView * vertexPos).xyz;
    // simple assignments
    tangent = vertexTangent;
    bitangent = vertexBitangent;
    texCoord = vertexTexCoord;
    gl_Position = matrix.modelViewProjection * vertexPos;
}