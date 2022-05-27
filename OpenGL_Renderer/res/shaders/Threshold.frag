#version 330 core
out vec4 FragColor; 
in vec2 TexCoords;

uniform sampler2D screenTexture;

vec3 levelRange(vec3 color, float minInput, float maxInput){
    return min(max(color - vec3(minInput), vec3(0.0)) / (vec3(maxInput) - vec3(minInput)), vec3(1.0));
}

void main()
{
    vec3 tex = texture(screenTexture, TexCoords).rgb;
    tex = levelRange(tex, 0.5f, 5.0f);
    FragColor = vec4(tex.rgb, 1.0);
    //float brightness = dot(tex.rgb, vec3(0.2126, 0.7152, 0.0722));
    //if(brightness > 1.0)
    //    FragColor = vec4(tex.rgb, 1.0);
    //else
    //    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}