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
	
	drawNonphoto_multi_fs4x.glsl
	Draw nonphotorealistic shading model for multiple lights.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variables for textures; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement nonphotorealistic shading model
//	Note: test all data and inbound values before using them!

out vec4 rtFragColor;

//custom uniforms
uniform vec3 uColor; 
uniform vec3 uCoolColor; 
uniform vec3 uWarmColor; 
uniform float uAlpha; 
uniform float uBeta; 


void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE BLUE
	rtFragColor = vec4(0.0, 0.0, 1.0, 1.0);

	
	 ///grab sample from texture
	vec4 diffuseSample = texture(uTex_dm, vPassTexcoord); 
	vec4 specularSample = texture(uTex_sm, vPassTexcoord);

	//DIFFUSE
	//normalize the vectors 
	 vec3 L = normalize(vPassLight); 
	 vec3 N = normalize (vPassNormal);
	 //get value from [-1, 1]
	 float diffuse = dot (N, L); 

	 //clamp to get rid of negative numbers 
	 diffuse = max(0.0, diffuse); 

	 //map diffuse value from [-1, 1] to [0, 1]
	// diffuse = serialize(diffuse); 


	
	
	/// GOOCH ////
	vec3 coolColor = vec3(23.0/255, 104.0/255, 191.0/255); 
	vec3 warmColor = vec3(1.0, 106.0/255, 25.0/255); 

	//mix in cool colors
	vec3 coolColorMix = coolColor + uColor * uAlpha; 

	//mix in warm colors 
	vec3 warmColorMix = warmCOlor + uColor * uBeta; 

	//now, interpolate the cool and warm colors depending on the lighting intensity
	//lower intensity = more cool colors 
	vec3 colorOut = mix(coolColorMix, warmColorMix, diffuse); 



	/// DEBUGGING ///



}
