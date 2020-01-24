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

// REFERENCE = https://web.archive.org/web/20180814113122/http://sunandblackcat.com/tipFullView.php?l=eng&topicid=27&topic=Hatching-And-Gooch-Shading-GLSL//referenced code from taking this class 2 years ago 



#version 410

// ****TO-DO: 
//	1) declare uniform variables for textures; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement Phong shading model
//	Note: test all data and inbound values before using them!


//3) declare inbound varying data
in vec3 vPassNormal; 
in vec3 vPassLight; 
in vec3 vPassView; 
in vec2 vPassTexcoord; 


//declare uniform variable for textures 
//dm for diffuse map 
uniform sampler2D uTex_dm; 

//sm at the end because it's a specular map; 
uniform sampler2D uTex_sm; 

out vec4 rtFragColor;

void main()
{



	// DUMMY OUTPUT: all fragments are OPAQUE GREEN
	rtFragColor = vec4(0.0, 1.0, 0.0, 1.0);




}
