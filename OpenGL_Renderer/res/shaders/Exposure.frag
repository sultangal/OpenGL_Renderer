#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float exposure;

void main()
{
	vec3 result = texture(screenTexture, TexCoords).rgb;
	result = result * exposure;
	FragColor = vec4(result,1.0);
}