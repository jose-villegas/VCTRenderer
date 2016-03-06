#version 430

out vec3 texCoord;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexTexCoord;

void main()
{
	gl_Position = vec4(vertexPosition, 1.0f);
	texCoord = vertexTexCoord;
}