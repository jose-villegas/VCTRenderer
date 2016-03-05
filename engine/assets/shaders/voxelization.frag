# version 430

in GeometryOut
{
    vec3 position;
    vec3 normal;
    vec3 texCoord;
    flat vec4 triangleAABB;
    flat int selectedAxis;
} In;

layout (location = 0) out vec4 fragColor;
layout (pixel_center_integer) in vec4 gl_FragCoord;

// atomic counter 
layout (binding = 0, offset = 0) uniform atomic_uint voxelFragCount;

uniform layout(binding = 0, rgb10_a2ui) uimageBuffer voxelPosition;
uniform layout(binding = 1, rgba8) imageBuffer voxelAlbedo;
uniform layout(binding = 2, rgba16f) imageBuffer voxelNormal;

uniform sampler2D diffuseMap;
uniform uint cellSize[2];
uniform uint storeMode = 1;

void main()
{
    if( In.position.x < In.triangleAABB.x || In.position.y < In.triangleAABB.y || 
    	In.position.x > In.triangleAABB.z || In.position.y > In.triangleAABB.w )
	{
		discard;
	}

	uvec4 temp = uvec4( gl_FragCoord.x, gl_FragCoord.y, cellSize[0] * gl_FragCoord.z, 0 ) ;
	uvec4 texcoord;

	if( In.selectedAxis == 0 )
	{
	    texcoord.x = cellSize[0] - temp.z;
		texcoord.z = temp.x;
		texcoord.y = temp.y;
	}
	else if( In.selectedAxis == 1 )
    {
	    texcoord.z = temp.y;
		texcoord.y = cellSize[0] - temp.z;
		texcoord.x = temp.x;
	}
	else
	    texcoord = temp;

	uint idx = atomicCounterIncrement( voxelFragCount );

	if(storeMode == 1)
	{
		vec3 N, C;

		N = In.normal;
		C = texture( diffuseMap, In.texCoord.xy ).rgb;

		imageStore( voxelPosition, int(idx), texcoord );
		imageStore( voxelNormal, int(idx), vec4(N, 0.0f) );
		imageStore( voxelAlbedo, int(idx), vec4(C, 0.0f) );
	}
}