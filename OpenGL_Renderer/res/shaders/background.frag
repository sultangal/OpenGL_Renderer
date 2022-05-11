#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

vec3 tonemapFilmic(vec3 x) {
  vec3 X = max(vec3(0.0), x - 0.004);
  vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
  return pow(result, vec3(2.2));
}

void main()
{		
    vec3 envColor = textureLod(environmentMap, WorldPos, 0.0).rgb;
	envColor = envColor * 5.0;
	vec3 mapped = tonemapFilmic(envColor);
	FragColor = vec4(mapped, 1.0);
}