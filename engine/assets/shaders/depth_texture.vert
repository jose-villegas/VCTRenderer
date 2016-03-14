#version 430

layout(location = 0) in vec3 vertexPosition;

uniform struct Matrices
{
    mat4 modelViewProjection;
} matrices;

void main()
{
    vec4 vertexPos = vec4(vertexPosition, 1.0);
    // final drawing pos
    gl_Position = matrices.modelViewProjection * vertexPos;
}