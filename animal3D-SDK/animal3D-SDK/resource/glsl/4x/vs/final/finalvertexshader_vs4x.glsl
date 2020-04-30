/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	passColor_transform_vs4x.glsl
	Transform position attribute and pass color attribute down the pipeline.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variable for MVP matrix; see demo code for hint
//	2) correctly transform input position by MVP matrix
//	3) declare attribute for vertex color input
//	4) declare varying to pass color input to fragment shader
//	5) assign vertex color input to varying

layout (location = 0) in vec4 aPosition;
layout (location = 3) in vec4 aColor;	// (3)

uniform mat4 uMVP;	// (1)

out vec4 vColor;	// (4)

void main()
{
	// DUMMY OUTPUT: directly assign input position to output position
	gl_Position = aPosition;
//	gl_Position = uMVP * aPosition;	// (2)
//	vColor = aColor;	// (5)
}