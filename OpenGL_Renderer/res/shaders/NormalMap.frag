#version 330 core
out vec4 OUT;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D specularTexture;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 ambientLight;

uniform float constant;
uniform float linear;
uniform float quadratic;
uniform vec3 lightColor;

void main()
{           
     // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;

    //attenuation
    float distance = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(10.2) * spec * lightColor;
    OUT = vec4((ambientLight + diffuse * attenuation + texture(specularTexture, fs_in.TexCoords).rgb * specular * attenuation) * color, 1.0);
}