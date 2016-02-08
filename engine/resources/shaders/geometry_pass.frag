#version 330

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gAlbedo;
layout(location = 3) out vec3 gSpecular;

in vec3 position;
in vec3 texCoord;
in vec3 normal;

in vec3 normalView;
in vec3 tangent;
in vec3 bitangent;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    vec3 transparent;

    float opacity;
    float shininess;
    float shininessStrenght;
    float refractionIndex;

    uint useNormalsMap;
};

uniform Material material;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalsMap;

uniform float alphaCutoff = 0.1;

vec3 normalMapping()                                                                     
{                                                                                           
    vec3 norm = normalize(normalView);
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
    gAlbedo = diffuseColor.rgb * material.diffuse;

    // store specular intensity
    vec4 specularColor = texture(specularMap, texCoord.xy);
    gSpecular = specularColor.rgb * material.specular;

    // store fragment position in gbuffer texture
    gPosition = position;
    // store per fragment normal
    gNormal = material.useNormalsMap == 1 ? normalMapping() : normalize(normal);
}