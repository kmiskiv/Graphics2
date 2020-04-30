/*
Made for final project
Kayla Miskiv
Spring 2020

Based on the files for the a3_Demo_Shading
*/

#include "../a3_Demo_Triangle.h"

typedef struct a3_DemoState a3_DemoState;

//-----------------------------------------------------------------------------

void a3triangle_init(a3_DemoState const* demoState, a3_Demo_Triangle* demoMode)
{
	demoMode->render = triangle_renderSolid;
	demoMode->display = triangle_displayTexture;
	demoMode->activeCamera = triangle_cameraSceneViewer;

	demoMode->pipeline = triangle_back;

	demoMode->targetIndex[triangle_back] = triangle_back_composite;
	demoMode->targetIndex[triangle_fbo] = triangle_fbo_composite;

	demoMode->targetCount[triangle_back] = triangle_target_back_max;
	demoMode->targetCount[triangle_fbo] = triangle_target_fbo_max;
}


//-----------------------------------------------------------------------------