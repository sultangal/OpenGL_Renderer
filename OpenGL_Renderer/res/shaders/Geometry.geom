#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 g_Normals;
//out vec4 g_Color;
out vec2 g_TexCoord;
out vec3 g_FragPos;
out mat3 g_TBN;

in vec3 v_Normals[];
in vec2 v_TexCoord[];
in vec3 v_FragPos[];
in mat4 v_Projection[];
in mat4 v_Model[];

uniform mat4 model;

void main()
{
    // Edges of the triangle
    vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    // Lengths of UV differences
    vec2 deltaUV0 = v_TexCoord[1] - v_TexCoord[0];
    vec2 deltaUV1 = v_TexCoord[2] - v_TexCoord[0];

    // one over the determinant
    float invDet = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    vec3 tangent = vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
    vec3 bitangent = vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));

    vec3 T = normalize(vec3(v_Model[0] * vec4(tangent, 0.0f)));
    vec3 B = normalize(vec3(v_Model[0] * vec4(bitangent, 0.0f)));
    vec3 N = normalize(vec3(v_Model[0] * vec4(cross(edge1, edge0), 0.0f)));

    mat3 TBN = mat3(T, B, N);
    // TBN is an orthogonal matrix and so its inverse is equal to its transpose
    TBN = transpose(TBN);
    ///////////////
    gl_Position =  gl_in[0].gl_Position;
    g_FragPos = v_FragPos[0];
    g_Normals = v_Normals[0];
    g_TexCoord = v_TexCoord[0];
    g_TBN = TBN;
    // Change all lighting variables to TBN space
   // crntPos = TBN * gl_in[0].gl_Position.xyz;
   // lightPos = TBN * data_in[0].lightPos;
   // camPos = TBN * data_in[0].camPos;

    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    g_FragPos = v_FragPos[1];
    g_Normals = v_Normals[1];
    g_TexCoord = v_TexCoord[1];
    g_TBN = TBN;
    EmitVertex();

    gl_Position =  gl_in[2].gl_Position;
    g_FragPos = v_FragPos[2];
    g_Normals = v_Normals[2];
    g_TexCoord = v_TexCoord[2];
    g_TBN = TBN;
    EmitVertex();

    EndPrimitive();
}