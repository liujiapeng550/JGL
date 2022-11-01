#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D baseMap;
//uniform sampler2D noiseMap;
uniform sampler2D waterbumpMap;
//uniform sampler2D maskMap;
uniform sampler2D RippleTexture;


uniform float time;
uniform vec3 _RiverParam01;
uniform vec3 _NormalParam;


vec3 UnpackScaleNormal(vec4 normalmap, float bumpScale)
{
	vec3 normal;
	normal.xy = (normalmap.xy * 2.0 - 1.0) * bumpScale;
	normal.z = sqrt(1.0 - clamp(dot(normal.xy, normal.xy), 0.0, 1.0));
	return normal;
}

void main()
{

	vec4 albedo = texture(baseMap, TexCoord);

	vec2 uv = TexCoord * _RiverParam01.x + vec2(-time * _RiverParam01.y, time * _RiverParam01.y)*0.5;
	vec4 rivernoiseuv = texture(waterbumpMap, uv)*0.15;
	//uvmask mask的效果为边缘为uv边缘为0，越靠进中心为1,如果没有这个处理的话，边缘的uv采样错误，平面最上面的会采样到图片下方的像素。
	float uvmask = TexCoord.x * TexCoord.y * (1.0 - TexCoord.x) * (1.0 - TexCoord.y);
	uvmask = uvmask * 20;
	uv = mix(TexCoord + rivernoiseuv.xy* uvmask, TexCoord, 1.0- albedo.a);

//法线计算{
	vec4  rivern = texture(waterbumpMap, uv* _RiverParam01.z);
	vec3 rivernormaldir = UnpackScaleNormal(rivern, 1);
	vec3 normal = UnpackScaleNormal(vec4(0.0, 0.0, 1.0, 1.0), _NormalParam.x);
	rivernormaldir = mix(vec3(0.0, 0.0, 1.0), rivernormaldir, _RiverParam01.z);
	normal = mix(normal, rivernormaldir, albedo.a);
	//}
//高光计算{

	vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0));
	vec3 viewDir = normalize(vec3(0.5, 0.5, 1.0));
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//}

//雨滴{
	float weight = 0.5;

	vec2 rippleuv = vec2(TexCoord.x / 2.2, TexCoord.y / 2.0)*20.0;
	vec4 Ripple = texture(RippleTexture, rippleuv);
	//w
	float DropFrac = fract(Ripple.w + time);

	float TimeFrac = DropFrac - 1.0f + Ripple.b;
	float DropFactor = clamp(0.2 + weight * 1.0 - DropFrac,0.0,1.0);
	float FinalFactor = DropFactor * Ripple.b * sin(clamp(TimeFrac * 9.0, 0.0f, 3.0f) * 3.1415926);
	Ripple.rg = Ripple.rg * 2 - 1;

	vec3 finalRipple = vec3(FinalFactor)*Ripple.a;
//}

	FragColor = texture(baseMap, uv) + spec * 1.2+ vec4(finalRipple, 1.0);

}