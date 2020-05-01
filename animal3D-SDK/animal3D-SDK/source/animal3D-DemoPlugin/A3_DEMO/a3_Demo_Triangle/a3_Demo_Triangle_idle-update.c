/*
Made for final project
Kayla Miskiv
Spring 2020

Based on the files for the a3_Demo_Pipelines

*/

#include "../a3_Demo_Triangle.h"

#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"


//-----------------------------------------------------------------------------
// UPDATE

void a3triangle_update(a3_DemoState* demoState, a3_Demo_Pipelines* demoMode, a3f64 dt)
{
	a3ui32 i;

	//active camera
	const a3_DemoProjector* activeCamera = demoState->projector + demoState->activeCamera;
	const a3_DemoSceneObject* activeCameraObject = activeCamera->sceneObject;

	a3_DemoPointLight* pointLight;


	a3_DemoModelMatrixStack matrixStack[demoStateMaxCount_sceneObject];


	// update matrix stack data
	for (i = 0; i < demoStateMaxCount_sceneObject; ++i)
	{
		a3demo_updateModelMatrixStack(matrixStack + i,
			activeCamera->projectionMat.m, activeCameraObject->modelMat.m, activeCameraObject->modelMatInv.m,
			(demoState->sceneObject + i)->modelMat.m, a3mat4_identity.m);
	}

	// send matrix stack data
	a3bufferRefill(demoState->ubo_transformStack_model, 0, sizeof(matrixStack), matrixStack);


	// send point light data
	pointLight = demoState->forwardPointLight;
	a3bufferRefill(demoState->ubo_pointLight, 0, demoState->forwardLightCount * sizeof(a3_DemoPointLight), pointLight);


}
