#version 430

layout(binding = 0) uniform sampler2D lightComposite;

out vec4 fragColor;

in vec2 texCoord;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(lightComposite, texCoord).rgb;
  
    // Reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // Gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    fragColor = vec4(mapped, 1.0);
}