#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
// material parameters
uniform sampler2D baseMap;
void main()
{
  vec4 albedo = texture(baseMap, TexCoords);
  FragColor = albedo;
}
