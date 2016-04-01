#version 330
in vec2 texCoord;

uniform sampler2D source;

uniform int blurType = 0;
uniform vec2 blurDirection;

out vec4 outColor;

vec4 blur13(sampler2D image, vec2 uv, vec2 direction) 
{
	vec4 color = vec4(0.0);
	vec2 off1 = vec2(1.411764705882353) * direction;
	vec2 off2 = vec2(3.2941176470588234) * direction;
	vec2 off3 = vec2(5.176470588235294) * direction;
	color += texture2D(image, uv) * 0.1964825501511404;
	color += texture2D(image, uv + off1) * 0.2969069646728344;
	color += texture2D(image, uv - off1) * 0.2969069646728344;
	color += texture2D(image, uv + off2) * 0.09447039785044732;
	color += texture2D(image, uv - off2) * 0.09447039785044732;
	color += texture2D(image, uv + off3) * 0.010381362401148057;
	color += texture2D(image, uv - off3) * 0.010381362401148057;
	return color;
}

vec4 blur5(sampler2D image, vec2 uv, vec2 direction) 
{
	vec4 color = vec4(0.0);
	vec2 off1 = vec2(1.3333333333333333) * direction;
	color += texture2D(image, uv) * 0.29411764705882354;
	color += texture2D(image, uv + off1) * 0.35294117647058826;
	color += texture2D(image, uv - off1) * 0.35294117647058826;
	return color; 
}

vec4 blur9(sampler2D image, vec2 uv, vec2 direction) {
	vec4 color = vec4(0.0);
	vec2 off1 = vec2(1.3846153846) * direction;
	vec2 off2 = vec2(3.2307692308) * direction;
	color += texture2D(image, uv) * 0.2270270270;
	color += texture2D(image, uv + off1) * 0.3162162162;
	color += texture2D(image, uv - off1) * 0.3162162162;
	color += texture2D(image, uv + off2) * 0.0702702703;
	color += texture2D(image, uv - off2) * 0.0702702703;
	return color;
}

void main()
{
	
	if(blurType == 1) 
		outColor = blur5(source, texCoord, blurDirection);
	else if(blurType == 2) 
		outColor = blur9(source, texCoord, blurDirection);
	else if(blurType == 3) 
		outColor = blur13(source, texCoord, blurDirection);
	else 
		outColor = blur5(source, texCoord, blurDirection);
};