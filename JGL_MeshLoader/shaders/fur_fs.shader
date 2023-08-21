#version 330 core
out vec4 FragColor;

in vec3 outNormal;
in vec2 TexCoord;


// texture sampler
uniform sampler2D FurNoiseTex;
uniform sampler2D FurColorTex;
//uniform sampler2D noiseMap;

void main()
{
	vec4 albedo = texture(FurNoiseTex, TexCoord);
	vec3 color = texture(FurColorTex, TexCoord).rgb;
	if (albedo.r> 0.1)
		discard;
	vec4 reult = vec4(color, albedo.r);
	
	FragColor = reult;
	//FragColor = vec4(finalRipple,1.0);
}