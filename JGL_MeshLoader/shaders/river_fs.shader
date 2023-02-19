#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D baseMap;
uniform sampler2D waterbumpMap;// 河流的法线贴图
uniform float time;
uniform vec3 _RiverParam01;//x:normal 水面的uv缩放 y:水面流动速度 z:水面法线uv缩放效果

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
	vec4 waterbumpUV = texture(waterbumpMap, uv)*0.15;
//根据a通道区分，河流和非河流区域
	uv = mix(TexCoord + waterbumpUV.xy, TexCoord, 1.0- albedo.a);
//法线计算{
	//河流的法线贴图
	vec4  rivern = texture(waterbumpMap, uv * _RiverParam01.z);
	//法线贴图获取法线信息
	vec3 rivernormaldir = UnpackScaleNormal(rivern, 1.0);
	vec3 normal = vec3(0.0, 0.0, 1.0);
	//根据a通道区分，河流和非河流区域
	normal = mix(normal, rivernormaldir, albedo.a);

//高光模拟计算{
	//phone 光照模型
	vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0));
	vec3 viewDir = normalize(vec3(0.5, 0.5, 1.0));
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	//}
	FragColor = texture(baseMap, uv) + spec * 1.2;
}