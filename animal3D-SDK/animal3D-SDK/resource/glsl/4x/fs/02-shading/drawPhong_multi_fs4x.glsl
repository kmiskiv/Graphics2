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

out vec4 rtFragColor;

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE GREEN
	rtFragColor = vec4(0.0, 1.0, 0.0, 1.0);

	/*

	//NOT REAL CODE
	kd = max (0.0, dot (...)); 


	ks = max(0.0, dot(...)); 
	ks *= ks; // ks^2
	ks *= ks // ks^4
	ks *= ks // ks^8
	ks *= ks // ks^16
	ks *= ks // ks^32
	ks *= ks // ks^64
	ks *= ks // ks^128

	vec2 rampCoord = vec2(kd, 0.0); 
	vec4 finalDiffuseColor = texture(uTex_dm_ramp, rampCoord); 
	*/
}
