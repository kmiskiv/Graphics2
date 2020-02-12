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
	
	passLightingData_shadowCoord_transform_vs4x.glsl
	Vertex shader that prepares and passes lighting data. Outputs transformed 
		position attribute and all others required for lighting. Also computes 
		and passes shadow coordinate.
*/

#version 410

// ****TO-DO: 
//	0) copy previous lighting data vertex shader
//	1) declare MVPB matrix for light
//	2) declare varying for shadow coordinate
//	3) calculate and pass shadow coordinate

//is a vec4 because it has a w value, which is 1
layout (location = 0) in vec4 aPosition;

// declare uniform variable for MV matrix
uniform mat4 uMV; 

// declare view position as outbound varying
out vec4 viewPos;

// declare uniform variable for P matrix
uniform mat4 uP;

// declare normal attribute 
layout (location = 2) in vec4 aNormal; 

// declare MV matrix for normals
uniform mat4 uMV_nrm; 

// declare outbound normal 
out vec4 vPassNormal; 

//declare Atlas transform
uniform mat4 uAtlas; 

//declare texture coordinate outbound varying
out vec2 vPassTextcoord; 

//declare texture coordinate attribute 
layout (location = 8) in vec4 inTextCoord; 

//declare uMVP matrix
uniform mat4 uMVP; 

//1) declare MVPB matrix for light
uniform mat4 uMVPB_other; 

//

//2) declare varying for shadow coordinate
out vec4 vShadowCoord; 

void main()
{
	
	//0 copy previous lighting data vertex shader

	// correctly transform input position by MV matrix to get view position
	viewPos = uMV * aPosition; 

	// correctly transform view position by P matrix to get final position 
		vec4 finalPos = uP * viewPos; 
 
		gl_Position = finalPos; 

	// correctly  transform input normal by MV normal matrix
	vPassNormal = uMV_nrm * aNormal; 

	// handle texture coordinate 

		//correctly transform input texture coordinate by atlas matrix 
		vPassTextcoord = (uAtlas * inTextCoord).xy;
	

	//3) calculate and pass shadow coordinate 
	//shadowcoord = shadow_matrix * position; 
	vShadowCoord = (uMVPB_other * aPosition ); 

	
	/// DEBUGGING /// 

	// DUMMY OUTPUT: directly assign input position to output position
	//gl_Position = aPosition;

}


