#version 440

out vec3 texCoord;

layout(location = 1) in vec3 vertexTexCoord;

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
    // fs quad uv coords
    texCoord = vertexTexCoord;
    // final drawing pos
    gl_Position = matrices.modelViewProjection * vertexPos;
}