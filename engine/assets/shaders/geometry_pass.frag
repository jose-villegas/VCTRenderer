#version 430

layout(location = 0) out vec3 gNormal;
layout(location = 1) out vec3 gAlbedo;
layout(location = 2) out vec4 gSpecular;
layout(location = 3) out vec3 gEmissive;

in Vertex
{
    vec3 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
};

uniform struct Material
{
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    float shininess;
    uint useNormalsMap;
} material;

layout(binding = 0) uniform sampler2D diffuseMap;
layout(binding = 1) uniform sampler2D specularMap;
layout(binding = 2) uniform sampler2D normalsMap;
layout(binding = 3) uniform sampler2D opacityMap;
layout(binding = 4) uniform sampler2D emissiveMap;

uniform float alphaCutoff = 0.01f;

vec3 normalMapping()
{
    vec3 norm = normalize(normal);
    vec3 tang = normalize(tangent);
    tang = normalize(tang - dot(tang, norm) * norm);
    vec3 bTan = normalize(bitangent);
    vec3 tNormal = texture(normalsMap, texCoord.xy).rgb;
    vec3 bumpedNormal = 2.0 * tNormal - vec3(1.0);
    mat3 TBN = mat3(tang, bTan, norm);
    return normalize(TBN * bumpedNormal);
}

void main()
{
    // store albedo rgb
    vec4 diffuseColor = texture(diffuseMap, texCoord.xy);
    float opacity = min(diffuseColor.a, texture(opacityMap, texCoord.xy).r);

    if (opacity <= alphaCutoff) { discard; }

    gAlbedo = diffuseColor.rgb * material.diffuse;
    // store specular intensity
    vec4 specularColor = texture(specularMap, texCoord.xy);
    // for emissive materials we ignore the specular
    gSpecular = vec4(specularColor.rgb * material.specular, max(material.shininess, 0.01f));
    // store per fragment normal
    gNormal = material.useNormalsMap == 1 ? normalMapping() : normalize(normal);
    // per fragment emissivenes
    gEmissive = material.emissive * texture(emissiveMap, texCoord.xy).rgb;
}