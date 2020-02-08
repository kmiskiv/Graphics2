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
	
	drawLambert_multi_mrt_fs4x.glsl
	Draw Lambert shading model for multiple lights with MRT output.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variable for texture; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement Lambert shading model
//	Note: test all data and inbound values before using them!
//	5) set location of final color render target (location 0)
//	6) declare render targets for each attribute and shading component
out vec4 rtFragColor;


//1) declare uniform variable for texture
uniform sampler2D uTex_dm; 

//2) declare uniform variables for lights 
uniform int uLightCt; 
uniform float uLightSz[4]; 
uniform float uLightSzInvSq[4]; 
uniform vec4 uLightPos[4]; 
uniform vec4 uLightCol[4]; 

//3) declare inbound varying data 
in vec4 viewPos; 
in vec4 vPassNormal; 
in vec2 vPassTextcoord; 

//5) set location of final color render target (location 0)
//rt for render target 
layout (location = 0) out vec4 rtColor; 

//6 declare render targets for each attribute and shading component
layout (location = 1) out vec4 rtViewPos; 
layout (location = 2) out vec4 rtViewNormal; 
layout (location = 3) out vec4 rtAtlasTextcoord; 
layout (location = 4) out vec4 rtDiffuseMap; 
//layout (location = 5) out vec4 rtSpecularMap; 
layout (location = 6) out vec4 rtDiffuseTotal; 
//layout (location = 7) out vec4 rtSpecularTotal;
layout (location = 8) out vec4 rtDepthBuffer;  

void main()
{
		//4) implement Lambert 
		
		//grab sample from texture 
		vec4 diffuseSample = texture(uTex_dm, vPassTextcoord); 
		
		//calculate the diffuse coefficient 
		vec3 N = normalize(vPassNormal.xyz); 
		vec3 diffuseTotal = vec3(0.0); 
		float diffuse = 0.0;
		vec3 L = vec3(0.0);  

		//the light loop 
		for (int i = 0; i < uLightCt; i++)
		{
			L = normalize(uLightPos[i].xyz - viewPos.xyz); 
		    diffuse = dot (N, L); 
			diffuse = max(diffuse, 0.0);
			diffuseTotal += diffuse * uLightCol[i].rgb;
		}


		//calculate Lambertian shading model 
		//shading * color 
		vec3 Lambert = diffuseTotal * diffuseSample.rgb; 

		//assign result to output color 
		//rtFragColor = vec4(Lambert, diffuseSample.a); 


		//output to render targets 
		rtColor = vec4(Lambert, diffuseSample.a); 
		rtViewPos = viewPos; 
		rtViewNormal = vec4(N, 1.0); 
		rtAtlasTextcoord = vec4(vPassTextcoord, 0.0, 1.0);
		rtDiffuseMap = diffuseSample;
		rtDiffuseTotal = vec4(diffuseTotal, 1.0); 


		// DEBUGGING //

	//rtFragColor = vec4(diffuse, diffuse, diffuse, 1.0); 


	// DUMMY OUTPUT: all fragments are OPAQUE RED
	//rtFragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

