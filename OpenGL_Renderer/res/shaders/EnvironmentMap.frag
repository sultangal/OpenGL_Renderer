#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;

uniform vec3 viewPos;
uniform samplerCube skybox;
uniform sampler2D normalMap;

void main()
{      
    //vec3 normal = texture(normalMap, TexCoords).rgb;
    //normal = mat3(transpose(inverse(model))) * normal;

    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}