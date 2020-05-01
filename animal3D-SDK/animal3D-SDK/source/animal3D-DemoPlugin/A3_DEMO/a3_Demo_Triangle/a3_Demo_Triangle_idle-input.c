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
		a3demoCtrlCasesLoop(demoMode->render, triangle_render_max, 'k', 'j');

		// toggle display program
		a3demoCtrlCasesLoop(demoMode->display, triangle_display_max, 'K', 'J');

		// toggle active camera
		a3demoCtrlCasesLoop(demoMode->activeCamera, triangle_camera_max, 'v', 'c');

		// toggle pipeline mode
		a3demoCtrlCasesLoop(demoMode->pipeline, triangle_pipeline_max, ']', '[');

		// toggle target
		a3demoCtrlCasesLoop(demoMode->targetIndex[demoMode->pass], demoMode->targetCount[demoMode->pass], '}', '{');

	case ')':
		a3demoCtrlIncLoop(demoMode->pass, triangle_pass_max);
	case 'I':
		if (demoState->skipIntermediatePasses)
		{
			a3demoCtrlIncClamp(demoMode->pass, triangle_passBlend, triangle_passComposite);
			a3demoCtrlIncClamp(demoMode->pass, triangle_passComposite, triangle_passScene);
			a3demoCtrlIncClamp(demoMode->pass, triangle_passScene, -1);
		}
		break;
	case '(':
		a3demoCtrlDecLoop(demoMode->pass, triangle_pass_max);
		if (demoState->skipIntermediatePasses)
		{
			a3demoCtrlDecClamp(demoMode->pass, triangle_passBlend, triangle_passComposite);
			a3demoCtrlDecClamp(demoMode->pass, triangle_passComposite, triangle_passScene);
			a3demoCtrlDecClamp(demoMode->pass, triangle_passScene, -1);
			demoMode->pass = (demoMode->pass + triangle_pass_max) % triangle_pass_max;
		}
		break;

	}

	// skip passes if not on the correct pipeline
	switch (asciiKey)
	{
	case ']':
	case '[':
	case ')':
		break;
	case '(':
		break;
	}

}
