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
	
	drawPhong_multi_fs4x.glsl
	Draw Phong shading model for multiple lights.
*/
#version 410

// ****TO-DO: 
//	1) declare uniform variables for textures; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement Phong shading model
//	Note: test all data and inbound values before using them!


//3) declare inbound varying data
in vec4 vPassNormal; 
in vec3 vPassLight; 
in vec3 vPassView; 
in vec2 vPassTexcoord; 
in vec4 viewPos; 

 //2) declare uniform variables for lights
 uniform int uLightCt; 
 uniform float uLightSz[4]; 
 uniform float uLightSzInvSq[4]; 
 uniform vec4 uLightPos[4]; 
 uniform vec4 uLightCol[4];


// 1) declare uniform variable for textures 
//dm for diffuse map 
uniform sampler2D uTex_dm; 
//sm at the end because it's a specular map; 
uniform sampler2D uTex_sm; 

out vec4 rtFragColor;

void main()
{
		//4) implement Phong shading model

		//grab sample from texture 
		vec4 diffuseSample = texture(uTex_dm, vPassTexcoord); 
		vec4 specularSample = texture(uTex_sm, vPassTexcoord);


	// 4) calculate diffuse coefficient 
	 
	vec3 N = normalize(vPassNormal.xyz); 
	 
	vec3 diffuseTotal = vec3(0.0); 
	vec3 specularTotal = vec3(0.0); 
	//SPECULAR 
	vec3 V = normalize(vPassView); 

	//the light loop
	for (int i = 0; i < uLightCt; i++)
	{
		vec3 L = normalize(uLightPos[i].xyz - viewPos.xyz);
		float diffuse = dot (N, L);	
		diffuse = max(diffuse, 0.0);
		diffuseTotal += diffuse * uLightCol[i].rgb;

		//calculate the reflection vector, diffuse constant, specular constant 


		//formula = 2( N dot L) N - L
		vec3 R = (diffuse + diffuse) * (N - L); 

		
		
		
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
	
	rtFragColor = vec4(Phong, diffuseSample.a); 

}
