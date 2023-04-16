
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;
in vec3 WorldPos;
//https://www.shadertoy.com/view/XlXGzB
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
		for (int j = 0; j < iterations; j++)
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


	vec2 iResolution = vec2(800, 600);
	vec2 iMouse = vec2(0, 0);
	float iTime = time;



	
    vec2 sc = 2.0 * TexCoord.xy / iResolution.xy - 1.0;
    sc.x *= iResolution.x / iResolution.y;

    vec3 mouse = 4.0 * vec3(2.0 * iMouse.xy / iResolution.xy - 1.0, 0.0);

    vec3 ro = vec3(0.0);

    bool key_m = texture(iChannel1, vec2(KEY_M, 0.75)).x > 0.0;
    ro = key_m ? 2.0 * vec3(earthRadius * sin(mouse.x), 0.0, earthRadius * cos(mouse.x)) :
        vec3(0.0, earthRadius + 0.1, 1000.0 * abs(cos(iTime / 10.0)));

    vec3 rd = normalize(rotate_y(vec3(sc, 1.2), M_PI - mouse.x));

    vec3 sun = normalize(vec3(1.0, 1.0, 1.0));

    vec4 col = inscatter(ro, rd, sun);

    vec3 sunPos = sun * distanceToSun;

    vec4 star = vec4(sunPos, sunRadius);
    vec2 t0 = iSphere(ro, rd, star);

    if (t0.x > 0.0)
    {
        col.xyz += vec3(1.0, 1.0, 1.0) * col.a;
    }

    vec3 stars = vec3(noise(rd * iResolution.y * 0.75));
    stars += vec3(noise(rd * iResolution.y * 0.5));
    stars += vec3(noise(rd * iResolution.y * 0.25));
    stars += vec3(noise(rd * iResolution.y * 0.1));
    stars = clamp(stars, 0.0, 1.0);
    stars = (vec3(1.0) - stars);

    col.xyz = col.xyz + stars * col.a;
    col.xyz = pow(col.xyz, vec3(1.0 / 2.2));

	FragColor = col;
}