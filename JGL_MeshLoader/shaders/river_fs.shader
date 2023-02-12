#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D baseMap;
//uniform sampler2D noiseMap;
uniform sampler2D waterbumpMap;



uniform float time;
uniform vec3 _RiverParam01;//x:normal ˮ���uv���� y:ˮ�������ٶ� z:ˮ�淨��uv����Ч��
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
	vec4 waterbumpUV = texture(waterbumpMap, uv)*0.15;
	//����aͨ�����֣������ͷǺ�������
	uv = mix(TexCoord + waterbumpUV.xy, TexCoord, 1.0- albedo.a);
//���߼���{
	vec4  rivern = texture(waterbumpMap, uv * _RiverParam01.z);
	vec3 rivernormaldir = UnpackScaleNormal(rivern, 1);
	vec3 normal = UnpackScaleNormal(vec4(0.0, 0.0, 1.0, 1.0), 0.0);
	rivernormaldir = mix(vec3(0.0, 0.0, 1.0), rivernormaldir, _RiverParam01.z);
	normal = mix(normal, rivernormaldir, albedo.a);

//�߹�ģ�����{
	vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0));
	vec3 viewDir = normalize(vec3(0.5, 0.5, 1.0));
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	//}
	FragColor = texture(baseMap, uv) + spec * 1.2;
}