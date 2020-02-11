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
	
	drawTexture_blurGaussian_fs4x.glsl
	Draw texture with Gaussian blurring.
*/

#version 410

// ****TO-DO: 
//	0) copy existing texturing shader
//	1) declare uniforms for pixel size and sampling axis
//	2) implement Gaussian blur function using a 1D kernel (hint: Pascal's triangle)
//	3) sample texture using Gaussian blur function and output result

uniform sampler2D uImage00;

layout (location = 0) out vec4 rtFragColor;

//put the degree on the end 
//this is the top of the triangle, so power of 0, so 1. 
vec4 blurGaussian0(in sampler2D img, in vec2 center, in vec2 direction)
{
		return texture(img, center); 
}

//third row of pascal's triangle
vec4 blurGaussian2(in sampler2D img, in vec2 center, in vec2 direction)
{
	vec4 total = vec4(0.0); 
	total += texture(img, center) * 2.0; 
	total += texture(img, center + direction); 
	total += texture(img, center - direction); 
	//return total divided by total weight, 2^2, so 4
	//return (total / 4.0); 
	//multiplication is faster
	return (total *0.25);
}


void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE MAGENTA
	rtFragColor = vec4(1.0, 0.0, 1.0, 1.0);
}
