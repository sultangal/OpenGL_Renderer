#version 330 core 
out vec4 OUT;

in vec3 v_Normals;//in vec4 g_Color;
in vec2 v_TexCoord;
in vec3 v_FragPos;
in mat3 v_TBN;

uniform vec3 u_Color;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 ambientLight;
uniform vec3 viewPos;
uniform vec3 directionalLight;

uniform float constant;
uniform float linear;
uniform float quadratic;

struct Spotlight {
    vec3  position;
    vec3  direction;
    float cutOff;
	float outerCutOff;
};  
uniform Spotlight spotlight;

float near = 0.1; 
float far  = 100.0; 


float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{	
	vec3 lightDir = normalize(lightPosition - v_FragPos);
	//vec3 colorrr = vec3(texture(texture1, v_TexCoord));   	
	//vec3 lightDir = normalize(-directionalLight);		
	
	//diffuse
	vec3 normal = normalize(v_Normals);
	//vec3 normal = normalize(texture(normalTexture, v_TexCoord).rgb * 2.0f - 1.0f);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//specular
	float specularStrength = 100.9;		
	vec3 viewDir = normalize(viewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;	

	//spotlight
	//float theta = dot(lightDir, normalize(-spotlight.direction));
	//float epsilon = (spotlight.cutOff - spotlight.outerCutOff);
	//float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);

	float distance = length(lightPosition - v_FragPos);
	float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance)); 
	
	//float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
	//OUT = vec4(vec3(depth), 1.0);
	//OUT = vec4((ambientLight + diffuse * intensity * attenuation*2 + texture(texture2, v_TexCoord).r * intensity * attenuation * specular ) * colorrr, 1.0f);
	
	OUT = vec4((ambientLight  + diffuse * attenuation + texture(specularTexture, v_TexCoord).rgb * specular * attenuation) * vec3(texture(diffuseTexture, v_TexCoord)), 1.0f);
	//OUT = vec4((ambientLight  + diffuse * attenuation + texture(specularTexture, v_TexCoord).rgb * specular * attenuation) * vec3(texture(diffuseTexture, v_TexCoord)), 1.0f);
	//OUT = vec4((ambientLight ) * colorrr, 1.0f);
};
