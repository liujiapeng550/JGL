/*
Copyright (c) 2018, Allegorithmic. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   * Neither the name of the Allegorithmic nor the
     names of its contributors may be used to endorse or promote products
     derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ALLEGORITHMIC BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//////////////////////////////// Fragment shader
#version 330

#include "../common/common.glsl"
#include "../common/uvtile.glsl"

in vec3 iFS_Normal;
in vec2 iFS_UV;
in vec3 iFS_PointWS;

out vec4 ocolor0;

uniform vec3 Lamp0Pos = vec3(0.0f,0.0f,70.0f);
uniform vec3 Lamp0Color = vec3(1.0f,1.0f,1.0f);
uniform float Lamp0Intensity = 1.0;
uniform vec3 Lamp1Pos = vec3(70.0f,0.0f,0.0f);
uniform vec3 Lamp1Color = vec3(0.198f,0.198f,0.198f);
uniform float Lamp1Intensity = 1.0;

uniform vec3 ambientColor = vec3(0.07f,0.07f,0.07f);
uniform float tiling = 1.0f;
uniform vec3 uvwScale = vec3(1.0, 1.0, 1.0);
uniform bool uvwScaleEnabled = false;
uniform bool sRGBBaseColor = true;

uniform sampler2D diffuseMap;

void main()
{
  vec3 pointToLight0DirWS = Lamp0Pos - iFS_PointWS;
  float pointToLight0Length = length(pointToLight0DirWS);
  pointToLight0DirWS *= 1.0 / pointToLight0Length;
  vec3 pointToLight1DirWS = Lamp1Pos - iFS_PointWS;
  float pointToLight1Length = length(pointToLight1DirWS);
  pointToLight1DirWS *= 1.0 / pointToLight1Length;

  // UV
  vec2 uv = iFS_UV;
  vec2 uvScale = vec2(1.0, 1.0);  
  if (uvwScaleEnabled)
    uvScale = uvwScale.xy;

	bool disableFragment = hasToDisableFragment(uv);
	uv = uv * tiling * uvScale;
	uv = getUDIMTileUV(uv);

  // Light contribution
  vec3 diffContrib0 = ( max(dot(pointToLight0DirWS, iFS_Normal), 0.0) *
      Lamp0Intensity * lampAttenuation(pointToLight0Length) ) * Lamp0Color;
  vec3 diffContrib1 = ( max(dot(pointToLight1DirWS, iFS_Normal), 0.0) *
      Lamp1Intensity * lampAttenuation(pointToLight1Length) ) * Lamp1Color;
  vec3 diffContrib = diffContrib0 + diffContrib1;

  // Diffuse
  vec3 diffuseColor = get2DSample(diffuseMap, uv, disableFragment, cDefaultColor.mBaseColor).rgb;
  if (sRGBBaseColor)
    diffuseColor = srgb_to_linear(diffuseColor);

  // Final Color
  vec3 finalcolor = diffuseColor*(diffContrib+ambientColor);

  // Final Color
  ocolor0 = vec4(finalcolor, 1.0);
}

