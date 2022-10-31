#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D baseMap;
//uniform sampler2D noiseMap;
uniform sampler2D waterbumpMap;
//uniform sampler2D maskMap;
uniform sampler2D bumpMap;


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
	//vec4 mask = texture(maskMap, TexCoord);
	vec4 bump = texture(bumpMap, TexCoord);
	vec4 albedo = texture(baseMap, TexCoord);
	//vec4 noise = texture(noiseMap, TexCoord);

	vec2 uv = TexCoord * _RiverParam01.x + time * _RiverParam01.y;
	vec4 rivernoiseuv = texture(waterbumpMap, uv)*0.15;
	float uvmask = TexCoord.x * TexCoord.y * (1.0 - TexCoord.x) * (1.0 - TexCoord.y);
	uvmask = uvmask * 10;
	//rivernoiseuv = mix(rivernoiseuv, vec4(0.0,0.0,0.0,0.0), uvmask)*0.15;
	uv = mix(TexCoord + rivernoiseuv.xy, TexCoord,  bump.a);

//∑®œﬂº∆À„{
	vec4  rivern = texture(bumpMap, rivernoiseuv.xy * _RiverParam01.z);
	vec3 rivernormaldir = UnpackScaleNormal(rivern, 1);
	vec3 normal = UnpackScaleNormal(vec4(bump.x, bump.y, 1.0, bump.w), _NormalParam.x);
	rivernormaldir = mix(vec3(0.0, 0.0, 1.0), rivernormaldir, _RiverParam01.z);
	normal = mix(normal, rivernormaldir, bump.a);
//}


	FragColor = texture(baseMap, uv);
}