#version 440

uniform struct Matrices {
    mat4 modelView;
    mat4 modelViewProjection;
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 normal;
} matrix;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out vec3 position;
out vec3 texCoord;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;

void main()
{
    vec4 vertexPos = vec4(vertexPosition, 1.0f);
    texCoord = vertexTexCoord;
    // space transform
    normal = vec3(normalize(vertexNormal));
    // position = (matrix.view * matrix.model * vertexPos).xyz;
    // model to camera view
    // tangent = (matrix.view * matrix.model * vec4(vertexTangent, 0.f)).xyz;
    // bitangent = (matrix.view * matrix.model * vec4(vertexBitangent, 0.f)).xyz;
    // final drawing pos
    gl_Position = matrix.modelViewProjection * vertexPos;
}