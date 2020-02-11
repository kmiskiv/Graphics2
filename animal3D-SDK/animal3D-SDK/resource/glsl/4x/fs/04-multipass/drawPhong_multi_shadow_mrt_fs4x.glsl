/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	drawPhong_multi_shadow_mrt_fs4x.glsl
	Draw Phong shading model for multiple lights with MRT output and 
		shadow mapping.
*/

#version 410

// ****TO-DO: 
//	0) copy existing Phong shader
//	1) receive shadow coordinate
//	2) perform perspective divide
//	3) declare shadow map texture
//	4) perform shadow test

// declare uniform variables for textures
//dm for diffuse map 
uniform sampler2D uTex_dm; 
//sm for specular map 
uniform sampler2D uTex_sm; 

// declare uniform variables for lights 
uniform int uLightCt; 
uniform float uLightSz[4]; 
uniform float uLightSzInvSq[4]; 
uniform vec4 uLightPos[4]; 
uniform vec4 uLightCol[4];

 // declare inbound varying data
in vec4 viewPos; 
in vec4 vPassNormal; 
in vec2 vPassTextcoord; 


//1) recieve shadow coordinate 
in vec4 vShadowCoord;  
 
 
// set location of final color render target (location 0)
layout (location = 0) out vec4 rtColor; 

 //declare render targets for each attribute and shading component 
layout (location = 1) out vec4 rtViewPos; 
layout (location = 2) out vec4 rtViewNormal; 
layout (location = 3) out vec4 rtAtlasTextcoord; 
layout (location = 4) out vec4 rtDiffuseMap; 
layout (location = 5) out vec4 rtSpecularMap; 
layout (location = 6) out vec4 rtDiffuseTotal; 
layout (location = 7) out vec4 rtSpecularTotal;
layout (location = 8) out vec4 rtDepthBuffer;  

out vec4 rtFragColor;

//3) declare shadow map texture 
uniform sampler2D uTex_shadow;  

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE GREEN
	//rtFragColor = vec4(0.0, 1.0, 0.0, 1.0);

	//0 implement Phong 
	//grab sample from texture 
	vec4 diffuseSample = texture(uTex_dm, vPassTextcoord); 
	vec4 specularSample = texture(uTex_sm, vPassTextcoord);
	 
	vec3 N = normalize(vPassNormal.xyz); 
	 
	vec3 diffuseTotal = vec3(0.0); 
	vec3 specularTotal = vec3(0.0); 
	vec3 L = vec3(0.0); 
	vec3 V = vec3(0.0);
	vec3 R = vec3(0.0); 

	//the light loop
	for (int i = 0; i < uLightCt; i++)
	{
		 L = normalize(uLightPos[i].xyz - viewPos.xyz);
		float diffuse = dot (N, L);	
		diffuse = max(diffuse, 0.0);
		diffuseTotal += diffuse * uLightCol[i].rgb;

		//calculate the reflection vector, diffuse constant, specular constant 

		//reflection vector
		//formula = 2( N dot L) N - L
		 R = (diffuse + diffuse) * (N - L); 

		//SPECULAR 
		 V = normalize(viewPos.xyz); 
		
		float specular = dot (V, R); 

		//clamp specular to get rid of negative numbers 
		specular = max(0.0, specular); 

		specularTotal += specular * uLightCol[i].rgb; 
	}

	//put together in a similar fashion as you put together diffuse 
	// 	PHONG
	//phong is the sum of diffuse, specular and ambient components. 
	vec3 Phong = (diffuseTotal * diffuseSample.rgb) 
				+ (specularTotal * specularSample.rgb)
				+  vec3(0.01, 0.0, 0.02); 
	
	rtColor = vec4(Phong, diffuseSample.a); 
	rtViewPos = viewPos; 
	rtViewNormal = vec4(N, 1.0); 
	rtAtlasTextcoord = vec4(vPassTextcoord, 0.0, 1.0); 
	rtDiffuseMap = diffuseSample; 
	rtSpecularMap = specularSample; 
	rtDiffuseTotal = vec4(diffuseTotal, 1.0);
	rtSpecularTotal = vec4(specularTotal, 1.0);

	//test 
	//rtFragColor = textureProj(uTex_shadow, vShadowCoord) * vec4(1.0); 
	

	//float shadowSample = texture2D(uTex_shadow, vShadowCoord);
	
	//2) perform perspective divide 
	vec4 perspectiveDivide = viewPos / viewPos.w; 

	//4) perform shadow test 
	//Perform the shadow test by comparing the coordinate depth against the shadow map sample.


}
