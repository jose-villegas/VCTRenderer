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
    vec3 transparent;
    float shininess;
    float refractionIndex;
} material;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D ambientMap;

void main()
{
    // store fragment position in gbuffer texture
    gPosition = position;
    // store per fragment normal
    gNormal = normalize(normal);
    // store albedo rgb
    gAlbedo = texture(ambientMap, texCoord.xy).rgb * material.ambient 
              + texture(diffuseMap, texCoord.xy).rgb * material.diffuse;
    // store specular intensity
    gSpecular = (texture(specularMap, texCoord.xy).rgb * material.specular).r;
}