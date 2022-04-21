#version 330 core
out vec4 FragColor; 
in vec2 TexCoords;

uniform sampler2D textureToBlend01;
uniform sampler2D textureToBlend02;

void main()
{
    vec3 texture01 = texture(textureToBlend01, TexCoords).rgb;
    vec3 texture02 = texture(textureToBlend02, TexCoords).rgb;
   FragColor = vec4(texture02, 1.0);
}