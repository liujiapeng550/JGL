#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

out vec3 WorldPos;
out vec3 Normal;
out vec4 Color;
out vec2 TexCoords;
void main()
{
	TexCoords = aTexCoords;
	Color = color;
	WorldPos = vec3(model * vec4(aPosition, 1.0));
	Normal = aNormal;

	gl_Position = projection * view * model * vec4(aPosition, 1.0f);

}