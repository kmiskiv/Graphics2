/*
Made for final project
Kayla Miskiv
Spring 2020

Based on the files for the a3_Demo_Shading
*/

#include "../a3_Demo_Triangle.h"

typedef struct a3_DemoState a3_DemoState;

#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"

//-----------------------------------------------------------------------------
// CALLBACKS

void a3triangleCB_input_keyCharPress(a3_DemoState const* demoState, a3_Demo_Triangle* demoMode, a3i32 asciiKey)
{

	switch (asciiKey)
	{
		// toggle render program
		a3demoCtrlCasesLoop(demoMode->render, pipelines_render_max, 'k', 'j');

		// toggle display program
		a3demoCtrlCasesLoop(demoMode->display, pipelines_display_max, 'K', 'J');

		// toggle active camera
		a3demoCtrlCasesLoop(demoMode->activeCamera, pipelines_camera_max, 'v', 'c');

		// toggle pipeline mode
		a3demoCtrlCasesLoop(demoMode->pipeline, pipelines_pipeline_max, ']', '[');

		// toggle target
		//a3demoCtrlCasesLoop(demoMode->targetIndex[demoMode->pass], demoMode->targetCount[demoMode->pass], '}', '{');
	}







}
