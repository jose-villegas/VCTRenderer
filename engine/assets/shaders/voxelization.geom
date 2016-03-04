#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Vertex
{
    vec3 position;
    vec3 texCoord;
    vec3 normal;
} In[3];

out GeometryOut
{
    vec3 position;
    vec3 normal;
    vec3 texCoord;
    flat vec4 triangleAABB;
    flat int selectedAxis;
} Out;

uniform mat4 viewProjections[3];
uniform uint cellSize[2];

int CalculateAxis()
{
	vec3 oneToZero = In[1].position - In[0].position;
	vec3 twoToZero = In[2].position - In[0].position;
	vec3 faceNormal = normalize(cross(oneToZero, twoToZero));

	float NdotXAxis = abs( faceNormal.x );
	float NdotYAxis = abs( faceNormal.y );
	float NdotZAxis = abs( faceNormal.z );

	if( NdotXAxis > NdotYAxis && NdotXAxis > NdotZAxis )
    {
		return 0;
	}
	else if( NdotYAxis > NdotXAxis && NdotYAxis > NdotZAxis  )
    {
	    return 1;
    }
	else
    {
	    return 2;
	}
}

vec4 EnlargedAxisAlignedBoundingBox(vec4 pos[3])
{
	vec4 axisAlignedBoundingBox;
	axisAlignedBoundingBox.xy = pos[0].xy;
	axisAlignedBoundingBox.zw = pos[0].xy;

	axisAlignedBoundingBox.xy = min(pos[1].xy, axisAlignedBoundingBox.xy);
	axisAlignedBoundingBox.zw = max(pos[1].xy, axisAlignedBoundingBox.zw);
	
	axisAlignedBoundingBox.xy = min(pos[2].xy, axisAlignedBoundingBox.xy);
	axisAlignedBoundingBox.zw = max(pos[2].xy, axisAlignedBoundingBox.zw);

	//Enlarge half-pixel
	vec2 hPixel = vec2(1.0f / cellSize[0], 1.0f / cellSize[1]);
	axisAlignedBoundingBox.xy -= hPixel;
	axisAlignedBoundingBox.zw += hPixel;

	return axisAlignedBoundingBox;
}

void main()
{
	int selectedIndex = CalculateAxis();
	mat4 projection = viewProjections[selectedIndex];
	Out.selectedAxis = selectedIndex;

	//transform vertices to clip space
	vec4 pos[3] = vec4[3]
	(
		projection * gl_in[0].gl_Position,
		projection * gl_in[1].gl_Position,
		projection * gl_in[2].gl_Position
	);

	// enlarge triangle for conservative voxelization
	Out.triangleAABB = EnlargedAxisAlignedBoundingBox(pos);
	// find 3 triangle edge plane
    vec3 e0 = vec3( pos[1].xy - pos[0].xy, 0 );
	vec3 e1 = vec3( pos[2].xy - pos[1].xy, 0 );
	vec3 e2 = vec3( pos[0].xy - pos[2].xy, 0 );
	vec3 n0 = cross( e0, vec3(0,0,1) );
	vec3 n1 = cross( e1, vec3(0,0,1) );
	vec3 n2 = cross( e2, vec3(0,0,1) );
	// dilate the triangle
	float pl = 1.4142135637309 / cellSize[0];
	pos[0].xy = pos[0].xy + pl * ( (e2.xy/dot(e2.xy,n0.xy)) + (e0.xy/dot(e0.xy,n2.xy)) );
	pos[1].xy = pos[1].xy + pl * ( (e0.xy/dot(e0.xy,n1.xy)) + (e1.xy/dot(e1.xy,n0.xy)) );
	pos[2].xy = pos[2].xy + pl * ( (e1.xy/dot(e1.xy,n2.xy)) + (e2.xy/dot(e2.xy,n1.xy)) );

	for(int i = 0; i < 3; ++i)
	{
		gl_Position = pos[i];
		Out.position = pos[i].xyz;
		Out.normal = In[i].normal;
		Out.texCoord = In[i].texCoord;

		EmitVertex();
	}

	EndPrimitive();
}