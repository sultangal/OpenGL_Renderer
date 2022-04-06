#version 330 core 
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Normals;

out vec3 v_Normals;
//out vec4 v_Color;
out vec2 v_TexCoord;
out vec3 v_FragPos;
out mat4 v_Projection;
out mat4 v_Model;

uniform float counter;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;
uniform vec3 camPosition;
void main()
{
 //v_Color = vec4(a_Color.x * counter, a_Color.y * counter, a_Color.z * counter, 1.0);
 v_TexCoord = a_TexCoord;

 //FragPos = vec3(trans * vec4(a_Position, 1.0));
 v_FragPos = vec3(model * vec4(a_Position, 1.0));
 v_Normals = inverse(transpose(mat3(model))) * a_Normals;
 v_Projection = projection;
 v_Model = projection * view;
 gl_Position = projection * view * vec4(v_FragPos, 1.0);
 //*vec4(FragPos, 1.0);
};