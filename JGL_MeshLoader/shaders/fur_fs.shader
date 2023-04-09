#version 330 core
out vec4 FragColor;

in vec3 outNormal;
in vec2 TexCoord;


// texture sampler
uniform sampler2D FurNoiseTex;
//uniform sampler2D noiseMap;

void main()
{
	vec4 albedo = texture(FurNoiseTex, TexCoord);
	if (albedo.r> 0.1)
		discard;
	vec4 color = vec4(1.0,1.0,0.0, 0.5);
	
	FragColor = albedo;
	//FragColor = vec4(finalRipple,1.0);
}