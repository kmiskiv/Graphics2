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
	
	drawLambert_multi_fs4x.glsl
	Draw Lambert shading model for multiple lights.
*/

#version 410

//from lambert vertex shader, but now inputs to fragment

//1) declare uniform variable for texture
//from texture fragment shader 
 uniform sampler2D uTex_dm; 

 //2) declare uniform variables for lights
 uniform int uLightCt; 
 uniform float uLightSz[4]; 
 uniform float uLightSzInvSq[4]; 
 uniform vec4 uLightPos[4]; 
 uniform vec4 uLightCol[4]; 


 //3) declare inbound varying data
//want a normal, position in viewspace, and textcoord from previous shader
in vec4 viewPos; 
in vec4 vPassNormal; 
in vec2 vPassTextcoord; 

out vec4 rtFragColor;

void main()
{
		
	//grab sample from texture 
	vec4 diffuseSample = texture(uTex_dm, vPassTextcoord); 


	// calculate diffuse coefficient 
	//vec3 L = normalize(uLightPos[0].xyz); 
	vec3 N = normalize(vPassNormal.xyz); 
	//float diffuse = dot(N, L); 
	vec3 diffuseTotal = vec3(0.0); 

	//light loop 
	for (int i = 0; i < uLightCt; i++)
	{
		vec3 L = normalize(uLightPos[i].xyz - viewPos.xyz);
		float diffuse = dot (N, L);	
		diffuse = max(diffuse, 0.0);
		diffuseTotal += diffuse * uLightCol[i].rgb; 
	}




	// calculate Labertian shading model 
	// shading * color 
	vec3 Lambert = diffuseTotal * diffuseSample.rgb; 


	//assign result to output color 
	rtFragColor = vec4(Lambert, diffuseSample.a); 


	// DEBUGGING // 

	//rtFragColor = vec4(diffuse, diffuse, diffuse, 1.0); 

	//takes range of -1 to 1 and converts it to range of 0 to 1
	//takes the normal and converts it to color 
	//this allows you to visualize data as color 
	//rtFragColor = vec4(N.xyz * 0.5 + 0.5, 1.0); 

	// can visualize light data as well
	//rtFragColor = vec4(L.xyz * 0.5 + 0.5, 1.0); 

}
