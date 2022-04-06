#shader vertex
#version 330 core 
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Normals;

out vec3 v_Normals;
out vec4 v_Color;
out vec2 v_TexCoord;
out vec3 FragPos;

uniform float counter;

uniform mat4 model;
uniform mat4 trans;
uniform mat4 view;
uniform mat4 projection;
void main()
{
 v_Color = vec4(a_Color.x * counter, a_Color.y * counter, a_Color.z * counter, 1.0);
 v_TexCoord = a_TexCoord;

 //FragPos = vec3(trans * vec4(a_Position, 1.0));
FragPos = vec3(model * vec4(a_Position, 1.0));
 v_Normals = inverse(transpose(mat3(model))) * a_Normals;
 gl_Position = projection * view * trans * vec4(a_Position, 1.0);
 //*vec4(FragPos, 1.0);

};

///////////////////////////////////////////////////////////////////////////////////////

//#geometry shader
//#version 330 core
//layout (triangles) in;
//layout(triangle_strip, max_vertices = 3) out;
///////////////////////////////////////////////////////////////////////////////////////

#shader fragment
#version 330 core

out vec4 OUT;

in vec3 FragPos;
in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normals;

uniform vec3 u_Color;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 ambientLight;
uniform vec3 viewPos;

void main()
{
	vec3 normal = normalize(v_Normals);
	vec3 lightDir = normalize(lightPosition - FragPos);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 5.9;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;
	
    //color = vec4(u_Color * diffuse, 1.0f);
	vec3 colorrr = vec3(texture(texture1, v_TexCoord));

	//color = vec4(colorrr *lightColor * (ambientLight + diffuse) + texture(texture2, v_TexCoord).r*specular, 1.0f);

	OUT = vec4((ambientLight + diffuse + texture(texture2, v_TexCoord).r * specular) * colorrr, 1.0f);
};