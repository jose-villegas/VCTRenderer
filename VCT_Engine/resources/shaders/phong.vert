#version 440
uniform struct Matrices
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

out vec3 normal;
out vec2 texCoord;
out vec3 position;

void main(void)
{
    vec4 vertexPos = vec4(vertexPosition, 1.0f);

    texCoord = vertexTexCoord;
    normal = normalize(matrix.normal * vec4(vertexNormal, 0.0f)).xyz;
    position = (matrix.modelView * vertexPos).xyz;
    
    gl_Position = matrix.modelViewProjection * vertexPos;
}