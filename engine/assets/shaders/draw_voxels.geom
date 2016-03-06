#version 430

// receive voxels points position
layout(points) in;
// outputs voxels as cubes
layout(triangle_strip, max_vertices = 24) out;

uniform struct Matrices
{
    mat4 viewProjection;
} matrices;

layout(binding = 0) uniform sampler3D voxelAlbedo;
uniform float halfVoxelSize;

in vec3 texCoord[];

out vec4 voxelColor;

void main()
{
	const vec4 cubeVertices[8] = vec4[8] 
	(
		vec4( 1.0,  1.0,  1.0, 1.0),
		vec4( 1.0,  1.0, -1.0, 1.0),
		vec4( 1.0, -1.0,  1.0, 1.0),
		vec4( 1.0, -1.0, -1.0, 1.0),
		vec4(-1.0,  1.0,  1.0, 1.0),
		vec4(-1.0,  1.0, -1.0, 1.0),
		vec4(-1.0, -1.0,  1.0, 1.0),
		vec4(-1.0, -1.0, -1.0, 1.0)
	);

	const int cubeIndices[24]  = int[24] 
	(
		0, 2, 1, 3, // right
		6, 4, 7, 5, // left
		5, 4, 1, 0, // up
		6, 7, 2, 3, // down
		4, 6, 0, 2, // front
		1, 3, 5, 7  // back
	);

	vec4 projectedVertices[8];

	for(int i = 0; i < 8; ++i)
	{
		projectedVertices[i] = matrices.viewProjection * (gl_in[0].gl_Position 
							   + (halfVoxelSize * cubeVertices[i]));
	}

	for(int face = 0; face < 6; ++face)
	{
		vec4 albedo = texture(voxelAlbedo, texCoord[0].xyz);

		if(albedo.a < 0.1f)
		{
			continue;
		}
	
		for(int vertex = 0; vertex < 4; ++vertex)
		{
			gl_Position = projectedVertices[cubeIndices[face * 4 + vertex]];
			voxelColor = albedo;
			EmitVertex();
		}

		EndPrimitive();
	}
}