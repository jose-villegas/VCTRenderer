#version 420

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Vertex
{
    vec3 position;
    vec3 texCoord;
    vec3 normal;
} In[3];

out Voxel
{
    vec3 position;
    vec3 normal;
    vec3 texCoord;
} VoxelOut;

uniform uint voxelTextureSize;
uniform mat4 voxelGridTransform;
uniform mat4 voxelGridInverseTransform;
uniform vec3 voxelGridSize;
uniform mat4 viewProjections[3];

uint CalculateProjectionAxis()
{
    const vec3 worldAxes[3] = vec3[3]
                              (
                                  vec3(1.0, 0.0, 0.0),
                                  vec3(0.0, 1.0, 0.0),
                                  vec3(0.0, 0.0, 1.0)
                              );
    uint projectionAxis = 0;
    float maximumArea = 0.0f;

    for (uint i = 0; i < 3; ++i)
    {
        float area = abs(dot(In[0].normal, worldAxes[i]));

        if (area > maximumArea)
        {
            maximumArea = area;
            projectionAxis = i;
        }
    }

    return projectionAxis;
}

void main()
{
    const vec3 worldAxes[3] = vec3[3]
                              (
                                  vec3(1.0, 0.0, 0.0),
                                  vec3(0.0, 1.0, 0.0),
                                  vec3(0.0, 0.0, 1.0)
                              );
    uint axisId = CalculateProjectionAxis();
    vec3 middle = (In[0].position + In[1].position + In[2].position) / 3.0f;
    vec3 projMiddle = (viewProjections[axisId] * vec4(middle, 1.0f)).xyz;
    vec3 voxelSize = voxelGridSize / voxelTextureSize;

    for (int i = 0; i < gl_in.length(); ++i)
    {
        vec3 projPos = (viewProjections[axisId] * vec4(In[i].position, 1.0f)).xyz;
        projPos += normalize(projPos - projMiddle) * (voxelSize.x / 2.0f);
        gl_Position = vec4(projPos, 1.0f);
        VoxelOut.position = (voxelGridInverseTransform * vec4(In[i].position,
                             1.0f)).xyz * 0.5f + 0.5f;
        VoxelOut.normal = In[i].normal;
        VoxelOut.texCoord = In[i].texCoord;
        EmitVertex();
    }

    EndPrimitive();
}