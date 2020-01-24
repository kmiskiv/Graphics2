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

// ****TO-DO: 
//	1) declare uniform variable for texture; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement Lambert shading model
//	Note: test all data and inbound values before using them!


//from lambert vertex shader, but now inputs to fragment
in vec3 vPassNormal; 
in vec3 vPassLight; 
in vec2 vPassTextcoord; 


//from texture fragment shader 
uniform sampler2D uText; 


out vec4 rtFragColor;

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE RED
	//rtFragColor = vec4(1.0, 0.0, 0.0, 1.0);


	//3) grab sample from texture 
	vec4 diffuseSample = texture(uText, vPassTextcoord); 


	// 4) calculate diffuse coefficient 
	vec3 L = normalize (vPassLight); 
	vec3 N = normalize (vPassNormal); 
	float diffuse = dot (N, L); 


	// calculate Labertian shading model 
	// shading * color 
	vec3 Lambert = diffuse * diffuseSample.rgb; 


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
