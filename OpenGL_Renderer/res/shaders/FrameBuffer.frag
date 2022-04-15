#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

vec3 aces(vec3 x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
void main()
{

    vec2 tex_offset = 1.0 / textureSize(screenTexture, 0); // gets size of single texel
    vec3 result = texture(screenTexture, TexCoords).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(screenTexture, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(screenTexture, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(screenTexture, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(screenTexture, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }


   ////tone mapping
   //const float gamma = 2.2;
   //vec3 mapped = aces(result);
   //mapped = pow(mapped, vec3(1.0 / gamma));
   ////out result
   FragColor = vec4(result, 1.0);
}


vec3 tonemapFilmic(vec3 x) {
  vec3 X = max(vec3(0.0), x - 0.004);
  vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
  return pow(result, vec3(2.2));
}

float tonemapFilmic(float x) {
  float X = max(0.0, x - 0.004);
  float result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
  return pow(result, 2.2);
}



float aces(float x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

//void main()
//{             
//    const float gamma = 1.0;
//    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
//  
//    // reinhard tone mapping
//    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
//    // gamma correction 
//    mapped = pow(mapped, vec3(1.0 / gamma));
//  
//    FragColor = vec4(mapped, 1.0);
//} 


//void main()
//{  
//    const float gamma = 2.2;
//    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
//
//    vec3 mapped = aces(hdrColor);
//     //gamma correction 
//    mapped = pow(mapped, vec3(1.0 / gamma));
//  
//    FragColor = vec4(mapped, 1.0);
//} 
//void main()
//{             
//    const float gamma = 2.2;
//    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
//  
//    // exposure tone mapping
//    vec3 mapped = vec3(1.0) - exp(-hdrColor * 4.0);
//    // gamma correction 
//    mapped = pow(mapped, vec3(1.0 / gamma));
//  
//    FragColor = vec4(mapped, 1.0);
//}  