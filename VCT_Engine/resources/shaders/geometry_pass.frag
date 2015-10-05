#version 440

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gAlbedo;
layout(location = 3) out float gSpecular;

in vec3 position;
in vec3 texCoord;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec3 normalView;

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    vec3 transparent;

    float opacity;
    float shininess;
    float shininessStrenght;
    float refractionIndex;

    uint shadingModel;
    uint blendMode;
} material;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

uniform float alphaCutoff = 0.1f;

void main()
{
    // store albedo rgb
    vec4 diffuseColor = texture(diffuseMap, texCoord.xy);

    if(diffuseColor.a <= alphaCutoff)
    {
        discard;
    }

    gAlbedo = diffuseColor.rgb * material.diffuse;
    // store specular intensity
    vec4 specularIntensity = texture(specularMap, texCoord.xy);
    gSpecular = (specularIntensity.rgb * material.specular).r;
    // store fragment position in gbuffer texture
    gPosition = position;
    // store per fragment normal
    gNormal = normalize(normal);
}