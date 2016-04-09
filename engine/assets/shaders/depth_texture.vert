#version 430

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexTexCoord;

uniform struct Matrices
{
    mat4 modelViewProjection;
} matrices;

out vec4 position;
out vec2 texCoord;

void main()
{
    vec4 vertexPos = vec4(vertexPosition, 1.0);
    position = matrices.modelViewProjection * vertexPos;
    texCoord = vertexTexCoord.xy;
    // final drawing pos
    gl_Position = position;
}