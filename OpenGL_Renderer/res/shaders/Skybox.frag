#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform sampler2D texture01;

void main()
{    

  FragColor = texture(skybox, TexCoords);
    //FragColor = texture(texture01, vec2(TexCoords));
   //FragColor = vec4(0.5, 1.0, 0.3, 1.0);
}