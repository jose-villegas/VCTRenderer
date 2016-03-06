# version 430

in GeometryOut
{
    vec3 position;
    vec3 normal;
    vec3 texCoord;
    flat int selectedAxis;
    flat vec4 triangleAABB;
} In;

layout (location = 0) out vec4 fragColor;
layout (pixel_center_integer) in vec4 gl_FragCoord;

uniform layout(binding = 0, rgba8) image3D voxelAlbedo;

uniform struct Material
{
    vec3 diffuse;
} material;

uniform sampler2D diffuseMap;
uniform uint volumeDimension;

void main()
{
    if( In.position.x < In.triangleAABB.x || In.position.y < In.triangleAABB.y || 
		In.position.x > In.triangleAABB.z || In.position.y > In.triangleAABB.w )
	{
		discard;
	}

	uvec3 temp = uvec3( gl_FragCoord.x, gl_FragCoord.y, volumeDimension * gl_FragCoord.z ) ;
	uvec3 texcoord;

	if( In.selectedAxis == 0 )
	{
	    texcoord.x = volumeDimension - temp.z;
		texcoord.z = temp.x;
		texcoord.y = temp.y;
	}
	else if( In.selectedAxis == 1 )
    {
	    texcoord.z = temp.y;
		texcoord.y = volumeDimension - temp.z;
		texcoord.x = temp.x;
	}
	else
	{
		texcoord = temp;
	}

	vec3 normal = In.normal;
	vec4 albedo = texture( diffuseMap, In.texCoord.xy );
	albedo = vec4(albedo.rgb * material.diffuse, albedo.a);

	imageStore(voxelAlbedo, ivec3(texcoord.xyz), albedo);
}