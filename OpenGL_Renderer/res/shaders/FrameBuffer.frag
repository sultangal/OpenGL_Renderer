#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;



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

vec3 aces(vec3 x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

float aces(float x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;

    vec3 mapped = tonemapFilmic(hdrColor*1.5);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
} 

//void main()
//{             
//    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
//    FragColor = vec4(hdrColor, 1.0);
//}  
//
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

