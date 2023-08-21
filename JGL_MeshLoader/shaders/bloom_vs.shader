#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

out vec3 WorldPos;
out vec3 Normal;
out vec2 TexCoords;
void main()
{
	TexCoords = aTexCoords;
	WorldPos = vec3(model * vec4(aPosition, 1.0));
	Normal = aNormal;

	gl_Position = projection * view * model * vec4(aPosition, 1.0f);

}