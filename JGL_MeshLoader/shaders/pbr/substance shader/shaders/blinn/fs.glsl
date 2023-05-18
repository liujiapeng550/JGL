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
#include "../common/parallax.glsl"
#include "../common/blinn_phong_shading.glsl"
#include "../common/uvtile.glsl"

in vec3 iFS_Normal;
in vec2 iFS_UV;
in vec3 iFS_Tangent;
in vec3 iFS_Binormal;
in vec3 iFS_PointWS;

out vec4 ocolor0;

uniform vec3 Lamp0Pos = vec3(0.0,0.0,70.0);
uniform vec3 Lamp0Color = vec3(1.0,1.0,1.0);
uniform float Lamp0Intensity = 1.0;
uniform vec3 Lamp1Pos = vec3(70.0,0.0,0.0);
uniform vec3 Lamp1Color = vec3(0.198,0.198,0.198);
uniform float Lamp1Intensity = 1.0;

uniform bool flipY = true;
uniform bool perFragBinormal = true;
uniform float Ks = 0.04;
uniform int parallax_mode = 0;
uniform float tessellationFactor = 4.0;
uniform float heightMapScale = 1.0;
uniform float Kr = 0.04;
uniform vec3 AmbiColor = vec3(0.07,0.07,0.07);
uniform float tiling = 1.0;
uniform vec3 uvwScale = vec3(1.0, 1.0, 1.0);
uniform bool uvwScaleEnabled = false;
uniform float glossMult = 0.5;
uniform float EmissiveIntensity = 1.0;

uniform float AmbiIntensity = 1.0;
uniform float envRotation = 0.0;

uniform sampler2D heightMap;
uniform sampler2D normalMap;
uniform sampler2D emissiveMap;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D opacityMap;
uniform sampler2D glossinessMap;
uniform samplerCube environmentMap;

uniform mat4 viewInverseMatrix;

void main()
{
	vec3 normalWS = iFS_Normal;
	vec3 tangentWS = iFS_Tangent;
	vec3 binormalWS = perFragBinormal ?
		fixBinormal(normalWS,tangentWS,iFS_Binormal) : iFS_Binormal;

	vec3 cameraPosWS = viewInverseMatrix[3].xyz;
	vec3 pointToLight0DirWS = Lamp0Pos - iFS_PointWS;
	float pointToLight0Length = length(pointToLight0DirWS);
	pointToLight0DirWS *= 1.0 / pointToLight0Length;
	vec3 pointToLight1DirWS = Lamp1Pos - iFS_PointWS;
	float pointToLight1Length = length(pointToLight1DirWS);
	pointToLight1DirWS *= 1.0 / pointToLight1Length;
	vec3 pointToCameraDirWS = normalize(cameraPosWS - iFS_PointWS);

	// ------------------------------------------
	// Parallax
	vec2 uvScale = vec2(1.0, 1.0);
	if (uvwScaleEnabled)
		uvScale = uvwScale.xy;
	vec2 uv = parallax_mode == 1 ? iFS_UV*tiling*uvScale : updateUV(
		heightMap,
		pointToCameraDirWS,
		normalWS, tangentWS, binormalWS,
		heightMapScale,
		iFS_UV,
		uvScale,
		tiling);

	uv = uv / tiling / uvScale;
	bool disableFragment = hasToDisableFragment(uv);
	uv = uv * tiling * uvScale;
	uv = getUDIMTileUV(uv);

	// ------------------------------------------
	// Add Normal from normalMap
	vec3 fixedNormalWS = normalWS;  // HACK for empty normal textures
  vec3 normalTS = get2DSample(normalMap, uv, disableFragment, cDefaultColor.mNormal).xyz;
	if(length(normalTS)>0.0001)
	{
		normalTS = fixNormalSample(normalTS,flipY);
		fixedNormalWS = normalize(
			normalTS.x*tangentWS +
			normalTS.y*binormalWS +
			normalTS.z*normalWS );
	}

  // ------------------------------------------
  // Compute Diffuse & Specular

  float glossiness = get2DSample(glossinessMap, uv, disableFragment, cDefaultColor.mGlossiness).r;
  glossiness = glossiness*glossMult;

  // Light 0 contribution
  vec3 diffContrib = vec3(0.0, 0.0, 0.0);
  vec3 specContrib = vec3(0.0, 0.0, 0.0);
  blinn_phong_shading(
      Lamp0Color*(Lamp0Intensity*lampAttenuation(pointToLight0Length)),
      fixedNormalWS,
      pointToLight0DirWS,
      pointToCameraDirWS,
      Ks,
      glossiness,
      diffContrib,
      specContrib);

  // Light 1 contribution
  vec3 diffContrib2 = vec3(0.0, 0.0, 0.0);
  vec3 specContrib2 = vec3(0.0, 0.0, 0.0);
  blinn_phong_shading(
      Lamp1Color*(Lamp1Intensity*lampAttenuation(pointToLight1Length)),
      fixedNormalWS,
      pointToLight1DirWS,
      pointToCameraDirWS,
      Ks,
      glossiness,
      diffContrib2,
      specContrib2);

  diffContrib += diffContrib2;
  specContrib += specContrib2;

  vec3 diffuseColor = get2DSample(diffuseMap, uv, disableFragment, cDefaultColor.mBaseColor).rgb;
  diffuseColor = srgb_to_linear(diffuseColor);

  vec3 specularColor = get2DSample(specularMap, uv, disableFragment, cDefaultColor.mSpecularColor).rgb;
  specularColor = srgb_to_linear(specularColor);
  vec3 R = reflect(-pointToCameraDirWS,fixedNormalWS);
  R = rotate(R,envRotation);
  vec3 reflColor = Kr * AmbiIntensity * texture(environmentMap,R).rgb;

  // ------------------------------------------
  vec3 emissiveContrib = get2DSample(emissiveMap, uv, disableFragment, cDefaultColor.mEmissive).rgb;
  emissiveContrib = srgb_to_linear(emissiveContrib) * EmissiveIntensity;

  vec3 finalcolor = specularColor*(specContrib+reflColor)
      + diffuseColor*(diffContrib+AmbiColor)
      + emissiveContrib;

  // Final Color
  float opacity = get2DSample(opacityMap, uv, disableFragment, cDefaultColor.mOpacity).r;
  ocolor0 = vec4(finalcolor, opacity);
}

