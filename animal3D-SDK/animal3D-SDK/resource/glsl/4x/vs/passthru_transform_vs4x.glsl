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
	
	passthru_transform_vs4x.glsl
	Pass-thru GLSL vertex shader. Outputs transformed position attribute.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variable for MVP matrix; see demo code for hint
//	2) correctly transform input position by MVP matrix

layout (location = 0) in vec4 aPosition;

uniform mat4 uMVP; 
//the correct declaration, do not need a layout/location
//for 4.3 and up: layout (location = 0) uniform mat4 uMVP
//all categories of data use different memory spaces 
//ie an attribute versus an uniform use different memory spaces 
//similar to doing offsets in assembly programming 
//attributes are measured in 4 float slots
//read the console!



void main()
{

	gl_Position = uMVP * aPosition;
	//why is it this way and not aPosition * uMVP?
	//order of operations for matrix multiplication 
	//openGL is right-handed 
	//input is always on the right, result is on the left. 
	//first thing that happens is on the right, and then reads leftwards. 

}

