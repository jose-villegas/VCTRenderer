#version 430

out vec2 texCoord;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;

void main()
{
    // fs quad uv coords
    texCoord = vertexTexCoord;
    // final drawing pos
    gl_Position = vec4(vertexPosition, 1.0);
}