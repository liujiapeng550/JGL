#version 330 core
out vec4 FragColor;

in vec3 outNormal;
in vec2 TexCoord;


// texture sampler
uniform sampler2D RippleTexture;
//uniform sampler2D noiseMap;



uniform float time;
uniform vec3 _RiverParam01;//x:normal 水面的uv缩放 y:水面流动速度 z:水面法线uv缩放效果
uniform vec3 _NormalParam;
uniform float _RippleSize;


void main()
{
	vec4 albedo = texture(RippleTexture, TexCoord);
	if (albedo.r> 0.1)
		discard;
	vec4 color = vec4(1.0,1.0,0.0, 0.5);
	
	FragColor = albedo;
	//FragColor = vec4(finalRipple,1.0);
}