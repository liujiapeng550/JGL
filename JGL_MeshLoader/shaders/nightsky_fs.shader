#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;
in vec3 WorldPos;

uniform float time;
uniform vec3 camPos;

void main()
{
	vec2 uv = TexCoord - 0.5;
	float time_in = time * 0.01;
	vec3 from = vec3(0.0, 0.0, 2.0);

	from += vec3(time_in * 2.0, time_in, -2.0);
	vec3 v = normalize(camPos - WorldPos);

	float s = 0.0;
	float formuparam = 0.53;
	float fade = 1.0;
	float brightness = 0.0015;
	float disfading = 0.73;
	float stepsize = 0.1;
	float saturation = 0.85;

	int volsteps = 20;
	int iterations = 20;
	vec3 dir = vec3(uv, 1.0);
	for (int i = 0; i < volsteps; i++)
	{
		vec3 p = from + dir * s;
		float pa, a = pa = 0.0;
		for (int j = 0; j < iterations;j++)
		{
			p = abs(p) / dot(p, p) - formuparam;
			a += abs(length(p) - pa);
			pa = length(p);
		}
		a *= a * a;
		v += vec3(s, s * s, s * s * s * s) * a * brightness * fade;
		fade *= disfading;
		s += stepsize;
	}
	v = mix(vec3(length(v), length(v), length(v)), v, saturation);
	FragColor = vec4(v * 0.01, 1.0);
}