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

	a3mat4* lightMVPptr, * lightMVPBptr;
	a3ui32 tmpLightCount, tmpBlockLightCount;

	a3_DemoPointLight* pointLight;

	// bias matrix
	const a3mat4 bias = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
	};

	// active camera
	a3_DemoProjector* activeCamera = demoState->projector + demoState->activeCamera;
	a3_DemoSceneObject* activeCameraObject = activeCamera->sceneObject;


	// send point light data
	pointLight = demoState->forwardPointLight;
	a3bufferRefill(demoState->ubo_pointLight, 0, demoState->forwardLightCount * sizeof(a3_DemoPointLight), pointLight);


	// update lights
	for (i = 0, pointLight = demoState->deferredPointLight + i,
		lightMVPptr = demoState->deferredLightMVP + i,
		lightMVPBptr = demoState->deferredLightMVPB + i;
		i < demoState->deferredLightCount;
		++i, ++pointLight, ++lightMVPptr, ++lightMVPBptr)
	{
		// set light scale and world position
		a3real4x4SetScale(lightMVPptr->m, pointLight->radius);
		lightMVPptr->v3 = pointLight->worldPos;

		// convert to view space and retrieve view position
		a3real4x4ConcatR(activeCameraObject->modelMatInv.m, lightMVPptr->m);
		pointLight->viewPos = lightMVPptr->v3;

		// complete by converting to clip space
		a3real4x4ConcatR(activeCamera->projectionMat.m, lightMVPptr->m);

		// calculate biased clip as well
		a3real4x4Product(lightMVPBptr->m, bias.m, lightMVPptr->m);
	}


	// upload buffer data
	tmpLightCount = demoState->deferredLightCount;
	if (tmpLightCount && demoMode->pipeline == pipelines_deferred_lighting)
	{
		demoState->deferredLightBlockCount = (tmpLightCount - 1) / demoStateMaxCount_lightVolumePerBlock + 1;
		for (i = 0, pointLight = demoState->deferredPointLight,
			lightMVPptr = demoState->deferredLightMVP, lightMVPBptr = demoState->deferredLightMVPB; i < demoState->deferredLightBlockCount;
			++i, tmpLightCount -= tmpBlockLightCount,
			pointLight += demoStateMaxCount_lightVolumePerBlock,
			lightMVPptr += demoStateMaxCount_lightVolumePerBlock, lightMVPBptr += demoStateMaxCount_lightVolumePerBlock)
		{
			// upload data for current light set
			tmpBlockLightCount = a3minimum(tmpLightCount, demoStateMaxCount_lightVolumePerBlock);
			demoState->deferredLightCountPerBlock[i] = tmpBlockLightCount;
			a3bufferRefill(demoState->ubo_transformMVP_light + i, 0, tmpBlockLightCount * sizeof(a3mat4), lightMVPptr);
			a3bufferRefill(demoState->ubo_transformMVPB_light + i, 0, tmpBlockLightCount * sizeof(a3mat4), lightMVPBptr);
			a3bufferRefill(demoState->ubo_pointLight + i, 0, tmpBlockLightCount * sizeof(a3_DemoPointLight), pointLight);
		}
	}
	else
	{
		demoState->deferredLightBlockCount = 0;
	}
}
