#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture01;
uniform sampler2D screenTexture02;

vec3 aces(vec3 x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp(((x * (a * x + b)) / (x * (c * x + d) + e)), 0.0, 1.0);
}

vec3 tonemapFilmic(vec3 x) {
  vec3 X = max(vec3(0.0), x - 0.004);
  vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
  return pow(result, vec3(2.2));
}

vec3 filmicToneMapping(vec3 color)
{
	color = max(vec3(0.), color - vec3(0.004));
	color = (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06);
	return color;
}

vec3 simpleReinhardToneMapping(vec3 color)
{
	float exposure = 5.5;
	color *= exposure/(1. + color / exposure);
	color = pow(color, vec3(1. / 2.2));
	return color;
}

vec3 whitePreservingLumaBasedReinhardToneMapping(vec3 color)
{
	float white = 2.;
	float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
	float toneMappedLuma = luma * (1. + luma / (white*white)) / (1. + luma);
	color *= toneMappedLuma / luma;
	color = pow(color, vec3(1. / 2.2));
	return color;
}

vec3 Uncharted2ToneMapping(vec3 color)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;
	float W = 11.2;
	float exposure = 15.;
	color *= exposure;
	color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
	float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
	color /= white;
	color = pow(color, vec3(1. / 2.2));
	return color;
}

void main()
{
	vec3 hdrColor01 = texture(screenTexture01, TexCoords).rgb;
	vec3 hdrColor02 = texture(screenTexture02, TexCoords).rgb;

	////vec3 mapped = vec3(1.0) - exp(-hdrColor * 1.0);
	vec3 result = hdrColor01+hdrColor02;
	//vec3 result = hdrColor01;
	//result = result * 1.0;
	vec3 mapped = aces(result);
	////mapped = pow(mapped, vec3( 1.0 / 1.0));
	////FragColor = vec4(mapped, 1.0);
	FragColor = vec4(mapped,1.0);
}