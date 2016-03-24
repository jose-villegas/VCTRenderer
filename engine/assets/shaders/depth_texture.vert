#version 430

layout(location = 0) in vec3 vertexPosition;

uniform struct Matrices
{
    mat4 modelViewProjection;
} matrices;

out vec4 position;

void main()
{
    vec4 vertexPos = vec4(vertexPosition, 1.0);
    // final drawing pos
    position = matrices.modelViewProjection * vertexPos;
    gl_Position = position;

}