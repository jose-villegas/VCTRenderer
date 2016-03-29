#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Vertex
{
	vec4 wsPosition;
    vec3 position;
    vec3 texCoord;
    vec3 normal;
} In[3];

out GeometryOut
{
	vec4 wsPosition;
    vec3 position;
    vec3 normal;
    vec3 texCoord;
    flat int selectedAxis;
    flat vec4 triangleAABB;
} Out;

uniform mat4 viewProjections[3];
uniform uint volumeDimension;

int CalculateAxis()
{
	vec3 oneToZero = In[1].position - In[0].position;
	vec3 twoToZero = In[2].position - In[0].position;
	vec3 faceNormal = cross(oneToZero, twoToZero);

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

vec4 EnlargedAxisAlignedBoundingBox(vec4 pos[3], vec2 pixelDiagonal)
{
	vec4 axisAlignedBoundingBox;

	axisAlignedBoundingBox.xy = min(pos[2].xy, min(pos[1].xy, pos[0].xy));
	axisAlignedBoundingBox.zw = max(pos[2].xy, max(pos[1].xy, pos[0].xy));

	// enlarge by half-pixel
	axisAlignedBoundingBox.xy -= pixelDiagonal;
	axisAlignedBoundingBox.zw += pixelDiagonal;

	return axisAlignedBoundingBox;
}

void main()
{
	int selectedIndex = CalculateAxis();
	mat4 viewProjection = viewProjections[selectedIndex];
	Out.selectedAxis = selectedIndex;

	//transform vertices to clip space
	vec4 pos[3] = vec4[3]
	(
		viewProjection * gl_in[0].gl_Position,
		viewProjection * gl_in[1].gl_Position,
		viewProjection * gl_in[2].gl_Position
	);

	vec2 halfPixel = vec2(1.0f / volumeDimension) * 0.5f;
	// calculate triangle aabb
	Out.triangleAABB = EnlargedAxisAlignedBoundingBox(pos, halfPixel);
	// find 3 triangle edge plane
    vec2 e0 = normalize( pos[1].xy - pos[0].xy );
	vec2 e1 = normalize( pos[2].xy - pos[1].xy );
	vec2 e2 = normalize( pos[0].xy - pos[2].xy );
	// dilate triangle for conservative voxelization
	pos[0].xy = pos[0].xy + normalize(-e0 + e2 ) * halfPixel;
	pos[1].xy = pos[1].xy + normalize( e0 - e1 ) * halfPixel;
	pos[2].xy = pos[2].xy + normalize( e1 - e2 ) * halfPixel;

	for(int i = 0; i < 3; ++i)
	{
		gl_Position = pos[i];
		Out.position = pos[i].xyz;
		Out.wsPosition = In[i].wsPosition;
		Out.normal = In[i].normal;
		Out.texCoord = In[i].texCoord;

		EmitVertex();
	}

	EndPrimitive();
}