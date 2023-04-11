#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
in vec3 Normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float PassIndex;
uniform float FurLength;
uniform vec3 vGravity;
//vec3 vGravity = vec3(1, -2.0, 0);

out vec2 TexCoord;
out vec3 outNormal;
void main()
{
	vec3  P = aPosition.xyz + (Normal * FurLength* PassIndex);
	outNormal = (model * vec4(Normal, 0.0)).xyz;
	vec3 gravity = (model * vec4(vGravity,0.0)).xyz;
	//设置每一层的位置偏移
	float K = pow(1.0-1.0/PassIndex, 3);
	P = P + gravity * K;
	TexCoord = vec2(aTexCoords.x, aTexCoords.y);
	gl_Position = projection * view * vec4(P,1.0f);
	//gl_Position = projection * view * model * vec4(aPosition, 1.0f);

}