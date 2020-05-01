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
	demoMode->render = triangle_renderPhong;
	demoMode->display = triangle_displayTexture;
	demoMode->activeCamera = triangle_cameraSceneViewer;

	demoMode->pipeline = triangle_forward;
	demoMode->pass = triangle_passComposite; 

	demoMode->targetIndex[triangle_passShadow] =	triangle_shadow_fragdepth;
	demoMode->targetIndex[triangle_passScene] =		triangle_scene_finalcolor;
	demoMode->targetIndex[triangle_passComposite] = triangle_composite_finalcolor;
	demoMode->targetIndex[triangle_passBright_2] =	triangle_bright_finalcolor;
	demoMode->targetIndex[triangle_passBlurH_2] =	triangle_blur_finalcolor;
	demoMode->targetIndex[triangle_passBlurV_2] =	triangle_blur_finalcolor;
	demoMode->targetIndex[triangle_passBright_4] =	triangle_bright_finalcolor;
	demoMode->targetIndex[triangle_passBlurH_4] =	triangle_blur_finalcolor;
	demoMode->targetIndex[triangle_passBlurV_4] =	triangle_blur_finalcolor;
	demoMode->targetIndex[triangle_passBright_8] =	triangle_bright_finalcolor;
	demoMode->targetIndex[triangle_passBlurH_8] =	triangle_blur_finalcolor;
	demoMode->targetIndex[triangle_passBlurV_8] =	triangle_blur_finalcolor;
	demoMode->targetIndex[triangle_passBlend] =		triangle_display_finalcolor;
													
	demoMode->targetCount[triangle_passShadow] =	triangle_target_shadow_max;
	demoMode->targetCount[triangle_passScene] =		triangle_target_scene_max;
	demoMode->targetCount[triangle_passComposite] = triangle_target_composite_max;
	demoMode->targetCount[triangle_passBright_2] =	triangle_target_bright_max;
	demoMode->targetCount[triangle_passBlurH_2] =	triangle_target_blur_max;
	demoMode->targetCount[triangle_passBlurV_2] =	triangle_target_blur_max;
	demoMode->targetCount[triangle_passBright_4] =	triangle_target_bright_max;
	demoMode->targetCount[triangle_passBlurH_4] =	triangle_target_blur_max;
	demoMode->targetCount[triangle_passBlurV_4] =	triangle_target_blur_max;
	demoMode->targetCount[triangle_passBright_8] =	triangle_target_bright_max;
	demoMode->targetCount[triangle_passBlurH_8] =	triangle_target_blur_max;
	demoMode->targetCount[triangle_passBlurV_8] =	triangle_target_blur_max;
	demoMode->targetCount[triangle_passBlend] =		triangle_target_display_max;

	
}


//-----------------------------------------------------------------------------