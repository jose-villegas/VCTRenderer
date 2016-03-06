#version 430

// receive voxels points position
layout(points) in;
// outputs voxels as cubes
layout(triangle_strip, max_vertices = 36) out;

uniform struct Matrices
{
    mat4 modelViewProjection;
} matrices;

in vec4 albedo[];

out vec4 voxelColor;
out vec3 worldNormal;

void main()
{
	vec4 cubeVertices[8] = vec4[8]
	(
		matrices.modelViewProjection * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.5, 0)),
		matrices.modelViewProjection * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0.5, 0)),
		matrices.modelViewProjection * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.5, 0)),
		matrices.modelViewProjection * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0.5, 0)),
		matrices.modelViewProjection * (gl_in[0].gl_Position + vec4(-0.5, 0.5, -0.5, 0)),
		matrices.modelViewProjection * (gl_in[0].gl_Position + vec4(0.5, 0.5, -0.5, 0)),
		matrices.modelViewProjection * (gl_in[0].gl_Position + vec4(-0.5, -0.5, -0.5, 0)),
		matrices.modelViewProjection * (gl_in[0].gl_Position + vec4(0.5, -0.5, -0.5, 0))
	);

	// front face
    worldNormal = vec3(0, 0, 1);
    gl_Position = cubeVertices[0];
    EmitVertex();
    gl_Position = cubeVertices[2];
    EmitVertex();
	gl_Position = cubeVertices[3];
    EmitVertex();
    EndPrimitive();
    gl_Position = cubeVertices[0];
    EmitVertex();
    gl_Position = cubeVertices[3];
    EmitVertex();
	gl_Position = cubeVertices[1];
    EmitVertex();
    EndPrimitive();

    // back face
    worldNormal = vec3(0, 0, -1);
    gl_Position = cubeVertices[5];
    EmitVertex();
    gl_Position = cubeVertices[7];
    EmitVertex();
	gl_Position = cubeVertices[6];
    EmitVertex();
    EndPrimitive();
    gl_Position = cubeVertices[5];
    EmitVertex();
    gl_Position = cubeVertices[6];
    EmitVertex();
	gl_Position = cubeVertices[4];
    EmitVertex();
    EndPrimitive();

    // right face
    worldNormal = vec3(1, 0, 0);
    gl_Position = cubeVertices[1];
    EmitVertex();
    gl_Position = cubeVertices[3];
    EmitVertex();
	gl_Position = cubeVertices[7];
    EmitVertex();
    EndPrimitive();
    gl_Position = cubeVertices[1];
    EmitVertex();
    gl_Position = cubeVertices[7];
    EmitVertex();
	gl_Position = cubeVertices[5];
    EmitVertex();
    EndPrimitive();

    // left face
    worldNormal = vec3(-1, 0, 0);
    gl_Position = cubeVertices[4];
    EmitVertex();
    gl_Position = cubeVertices[6];
    EmitVertex();
	gl_Position = cubeVertices[2];
    EmitVertex();
    EndPrimitive();
    gl_Position = cubeVertices[4];
    EmitVertex();
    gl_Position = cubeVertices[2];
    EmitVertex();
	gl_Position = cubeVertices[0];
    EmitVertex();
    EndPrimitive();

    // top face
    worldNormal = vec3(0, 1, 0);
    gl_Position = cubeVertices[4];
    EmitVertex();
    gl_Position = cubeVertices[0];
    EmitVertex();
	gl_Position = cubeVertices[1];
    EmitVertex();
    EndPrimitive();
    gl_Position = cubeVertices[4];
    EmitVertex();
    gl_Position = cubeVertices[1];
    EmitVertex();
	gl_Position = cubeVertices[5];
    EmitVertex();
    EndPrimitive();

    // bottom face
    worldNormal = vec3(0, -1, 0);
    gl_Position = cubeVertices[2];
    EmitVertex();
    gl_Position = cubeVertices[6];
    EmitVertex();
	gl_Position = cubeVertices[7];
    EmitVertex();
    EndPrimitive();
    gl_Position = cubeVertices[2];
    EmitVertex();
    gl_Position = cubeVertices[7];
    EmitVertex();
	gl_Position = cubeVertices[3];
    EmitVertex();
    EndPrimitive();
}