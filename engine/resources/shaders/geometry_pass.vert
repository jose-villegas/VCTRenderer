#version 330

out vec3 position;
out vec3 texCoord;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

struct Matrices
{
    mat4 modelView;
    mat4 modelViewProjection;
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 normal;
};

uniform Matrices matrices;

void main()
{
    vec4 vertexPos = vec4(vertexPosition, 1.0);
    vec4 vertexNorm = vec4(vertexNormal, 0.0);
    texCoord = vertexTexCoord;
    // space transform
    normal = (matrices.normal * vertexNorm).xyz;
    position = (matrices.modelView * vertexPos).xyz;
    // model to camera view
    tangent = (matrices.normal * vec4(vertexTangent, 0.0)).xyz;
    bitangent = (matrices.normal * vec4(vertexBitangent, 0.0)).xyz;
    // final drawing pos
    gl_Position = matrices.modelViewProjection * vertexPos;
}