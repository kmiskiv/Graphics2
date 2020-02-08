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
	
	drawTexture_fs4x.glsl
	Draw texture sample.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variable for texture; see demo code for hints
//	2) declare inbound varying for texture coordinate
//	3) sample texture using texture coordinate
//	4) assign sample to output color

out vec4 rtFragColor;

//1) delcare uniform variable for texture
uniform sampler2D uTex_dm; 

//2) declare inbound varying for texture coordinate 
in vec2 vPassTextcoord; 

void main()
{

	//3) sample texture using texture coordinate 
	vec4 diffuseSample = texture(uTex_dm, vPassTextcoord); 

	//4) assign sample to output color 
	rtFragColor = diffuseSample;

	// DEBUGGING //

	//rtFragColor = vec4(vPassTextcoord, 0.0, 1.0); 


	// DUMMY OUTPUT: all fragments are OPAQUE WHITE
	//rtFragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

