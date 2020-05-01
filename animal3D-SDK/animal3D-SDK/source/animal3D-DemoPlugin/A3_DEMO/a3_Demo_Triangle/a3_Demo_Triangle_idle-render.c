/*
Made for final project
Kayla Miskiv
Spring 2020

Based on the files for the a3_Demo_Shading
*/

#include "../a3_Demo_Triangle.h"

#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoRenderUtils.h"

// OpenGL
#ifdef _WIN32
#include <gl/glew.h>
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32

void a3triangle_render_controls(a3_DemoState const* demoState, a3_Demo_Triangle const* demoMode, a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{

	// display mode info
	a3byte const* pipelineText[triangle_pipeline_max] =
	{
		"Sierpinski's Triangle"
	};


	//forward pipeline names
	a3byte const* renderProgramName[triangle_render_max] =
	{
		"Phong Shading",
	};

	// forward display names
	a3byte const* displayProgramName[triangle_display_max] =
	{
		"Texture",

	};

	// active camera name
	a3byte const* cameraText[triangle_camera_max] = {
		"Scene camera",
		"Shadow map light",
	};

	// pass names
	a3byte const* passName[triangle_pass_max] =
	{
		"Pass: Capture shadow map",
		"Pass: Render scene objects",
		"Pass: Composite scene",
		"Pass: Bright pass (1/2 frame)",
		"Pass: Horizontal blur (1/2 frame)",
		"Pass: Vertical blur (1/2 frame)",
		"Pass: Bright pass (1/4 frame)",
		"Pass: Horizontal blur (1/4 frame)",
		"Pass: Vertical blur (1/4 frame)",
		"Pass: Bright pass (1/8 frame)",
		"Pass: Horizontal blur (1/8 frame)",
		"Pass: Vertical blur (1/8 frame)",
		"Pass: Bloom composite",

	};

	a3byte const* targetText_shadow[triangle_target_shadow_max] = {
	"Depth buffer",
	};
	a3byte const* targetText_scene[triangle_target_scene_max] = {
		"Color target 0: FINAL DISPLAY COLOR",
		"Color target 1: Attrib data: atlas texcoord",
		"Color target 2: Attrib data: view tangent",
		"Color target 3: Attrib data: view bitangent",
		"Color target 4: Attrib data: view normal",
		"Color target 5: Attrib data: view position",
		"Color target 6: Lighting: diffuse total",
		"Color target 7: Lighting: specular total",
		"Depth buffer",
	};

	a3byte const* targetText_composite[triangle_target_composite_max] = {
	"Color target 0: FINAL DISPLAY COLOR",
	};
	a3byte const* targetText_bright[triangle_target_bright_max] = {
		"Color target 0: FINAL DISPLAY COLOR",
		"Color target 1: Luminance",
	};
	a3byte const* targetText_blur[triangle_target_blur_max] = {
		"Color target 0: FINAL DISPLAY COLOR",
	};

	a3byte const* const* targetText[triangle_pass_max] = {
	targetText_shadow,
	targetText_scene,
	targetText_composite,
	targetText_bright,
	targetText_blur,
	targetText_blur,
	targetText_bright,
	targetText_blur,
	targetText_blur,
	targetText_bright,
	targetText_blur,
	targetText_blur,
	targetText_composite,
	};


	// text color
	a3vec4 const col = { a3real_half, a3real_zero, a3real_half, a3real_one };

	// pipeline and target
	a3_Demo_Triangle_RenderProgramName const render = demoMode->render;
	a3_Demo_Triangle_DisplayProgramName const display = demoMode->display;
	a3_Demo_Triangle_PipelineName const pipeline = demoMode->pipeline;
	a3_Demo_Triangle_PassName const pass = demoMode->pass; 
	a3_Demo_Triangle_TargetName const targetIndex = demoMode->targetIndex[pass];
	a3_Demo_Triangle_TargetName const targetCount = demoMode->targetCount[pass];
	a3_Demo_Triangle_ActiveCameraName const activeCamera = demoMode->activeCamera;
	
	
	// demo modes
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Pipeline (%u / %u) ('[' | ']'): %s", pipeline + 1, triangle_pipeline_max, pipelineText[pipeline]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Display pass (%u / %u) ('(' | ')'): %s", pass + 1, triangle_pass_max, passName[pass]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"        Target (%u / %u) ('{' | '}'): %s", targetIndex + 1, targetCount, targetText[pass][targetIndex]);

	// lighting modes
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Rendering mode (%u / %u) ('j' | 'k'): %s", render + 1, triangle_render_max, renderProgramName[render]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Display mode (%u / %u) ('J' | 'K'): %s", display + 1, triangle_display_max, displayProgramName[display]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Active camera (%u / %u) ('c' prev | next 'v'): %s", activeCamera + 1, triangle_camera_max, cameraText[activeCamera]);

}
	//-----------------------------------------------------------------------------


// sub-routine for rendering the demo state using the shading pipeline
void a3triangle_render(a3_DemoState const* demoState, a3_Demo_Triangle const* demoMode)
{
	// pointers
	const a3_VertexDrawable* currentDrawable;
	const a3_DemoStateShaderProgram* currentDemoProgram;

	// framebuffers
	const a3_Framebuffer* currentReadFBO,  * currentWriteFBO, * currentDisplayFBO;

	// indices
	a3ui32 i, j, k;

	// RGB
	const a3vec4 rgba4[] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f },	// red
		{ 0.0f, 1.0f, 0.0f, 1.0f },	// green
		{ 0.0f, 0.0f, 1.0f, 1.0f },	// blue
		{ 0.0f, 1.0f, 1.0f, 1.0f },	// cyan
		{ 1.0f, 0.0f, 1.0f, 1.0f },	// magenta
		{ 1.0f, 1.0f, 0.0f, 1.0f },	// yellow
		{ 1.0f, 0.5f, 0.0f, 1.0f },	// orange
		{ 0.0f, 0.5f, 1.0f, 1.0f },	// sky blue
		{ 0.5f, 0.5f, 0.5f, 1.0f },	// solid grey
		{ 0.5f, 0.5f, 0.5f, 0.5f },	// translucent grey
	};
	const a3real
		* const red = rgba4[0].v, * const green = rgba4[1].v, * const blue = rgba4[2].v,
		* const cyan = rgba4[3].v, * const magenta = rgba4[4].v, * const yellow = rgba4[5].v,
		* const orange = rgba4[6].v, * const skyblue = rgba4[7].v,
		* const grey = rgba4[8].v, * const grey_t = rgba4[9].v;

	// colorization levels
	const a3vec4 hueWheel[] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f },	// red
		{ 1.0f, 0.5f, 0.0f, 1.0f },	// orange
		{ 1.0f, 1.0f, 0.0f, 1.0f },	// yellow
		{ 0.5f, 1.0f, 0.0f, 1.0f },	// lime
		{ 0.0f, 1.0f, 0.0f, 1.0f },	// green
		{ 0.0f, 1.0f, 0.5f, 1.0f },	// aqua
		{ 0.0f, 1.0f, 1.0f, 1.0f },	// cyan
		{ 0.0f, 0.5f, 1.0f, 1.0f },	// sky
		{ 0.0f, 0.0f, 1.0f, 1.0f },	// blue
		{ 0.5f, 0.0f, 1.0f, 1.0f },	// purple
		{ 1.0f, 0.0f, 1.0f, 1.0f },	// magenta
		{ 1.0f, 0.0f, 0.5f, 1.0f },	// rose
	};

	const a3ui32 hueCount = sizeof(hueWheel) / sizeof(*hueWheel);


	// camera used for drawing
	const a3_DemoProjector* activeCamera = demoState->projector + demoState->activeCamera;
	const a3_DemoSceneObject* activeCameraObject = activeCamera->sceneObject;

	// shadow caster
	const a3_DemoProjector* activeShadowCaster = demoState->shadowLight;

	// current scene object being rendered, for convenience
	const a3_DemoSceneObject* currentSceneObject, * endSceneObject;

	//temp drawable pointers
	const a3_VertexDrawable* drawable[] =
	{
		demoState->draw_plane,
		demoState->draw_sphere,
		demoState->draw_cylinder,
		demoState->draw_torus,
		demoState->draw_triangle,
		demoState->draw_teapot,
	};

	// temp texture pointers
	const a3_Texture* texture_dm[] = {
		demoState->tex_stone_dm,
		demoState->tex_earth_dm,
		demoState->tex_stone_dm,
		demoState->tex_mars_dm,
		demoState->tex_checker,
		demoState->tex_earth2,
	};


	const a3_Texture* texture_sm[] =
	{
		demoState->tex_stone_dm,
		demoState->tex_earth_sm,
		demoState->tex_stone_dm,
		demoState->tex_mars_sm,
		demoState->tex_checker,
		demoState->tex_earth2,

	};

	// temp texture atlas matrix pointers
	const a3mat4* atlas[] = {
		demoState->atlas_stone,
		demoState->atlas_earth,
		demoState->atlas_stone,
		demoState->atlas_mars,
		demoState->atlas_checker,
	};


	// forward pipeline shader programs
	const a3_DemoStateShaderProgram* renderProgram[triangle_pipeline_max][triangle_render_max] = {
		{
			demoState->prog_drawPhong_multi_forward_mrt,

		},

	};

	//display shader programs
	const a3_DemoStateShaderProgram* displayProgram[triangle_display_max] =
	{
		{
			demoState->prog_drawTexture,
		},
	};


	//framebuffers to which to write based on pipeline mode
	const a3_Framebuffer* writeFBO[triangle_pass_max] =
	{
		demoState->fbo_shadow_d32,
		demoState->fbo_scene_c16d24s8_mrt,
		demoState->fbo_composite_c16 + 2,
		demoState->fbo_post_c16_2fr + 0,
		demoState->fbo_post_c16_2fr + 1,
		demoState->fbo_post_c16_2fr + 2,
		demoState->fbo_post_c16_4fr + 0,
		demoState->fbo_post_c16_4fr + 1,
		demoState->fbo_post_c16_4fr + 2,
		demoState->fbo_post_c16_8fr + 0,
		demoState->fbo_post_c16_8fr + 1,
		demoState->fbo_post_c16_8fr + 2,
		demoState->fbo_composite_c16 + 0,
	};

	// framebuffers from which to read based on pipeline mode
	const a3_Framebuffer* readFBO[triangle_pass_max][4] =
	{
		{ 0, },
		{ 0, demoState->fbo_shadow_d32, 0, },
		{ demoState->fbo_scene_c16d24s8_mrt, 0, },
		{ demoState->fbo_composite_c16 + 2, 0, },
		{ demoState->fbo_post_c16_2fr + 0, 0, },
		{ demoState->fbo_post_c16_2fr + 1, 0, },
		{ demoState->fbo_post_c16_2fr + 2, 0, },
		{ demoState->fbo_post_c16_4fr + 0, 0, },
		{ demoState->fbo_post_c16_4fr + 1, 0, },
		{ demoState->fbo_post_c16_4fr + 2, 0, },
		{ demoState->fbo_post_c16_8fr + 0, 0, },
		{ demoState->fbo_post_c16_8fr + 1, 0, },
		{ demoState->fbo_post_c16_8fr + 2, demoState->fbo_post_c16_4fr + 2, demoState->fbo_post_c16_2fr + 2, demoState->fbo_composite_c16 + 2, },

	};

	//target info
	a3_Demo_Triangle_RenderProgramName const render = demoMode->render;
	a3_Demo_Triangle_DisplayProgramName const display = demoMode->display;
	a3_Demo_Triangle_PipelineName const pipeline = demoMode->pipeline;
	a3_Demo_Triangle_PassName const pass = demoMode->pass; 
	a3_Demo_Triangle_TargetName const targetIndex = demoMode->targetIndex[pass], targetCount = demoMode->targetCount[pass];
	a3_Demo_Triangle_PassName currentPass; 


	// pixel size and effect axis
	a3vec2 pixelSize = a3vec2_one;
	a3vec2 sampleAxisH = { +a3real_one, +a3real_one };// a3vec2_x;
	a3vec2 sampleAxisV = { +a3real_one, -a3real_one };// a3vec2_y;



	// bias matrix
	const a3mat4 bias = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
	};
	const a3mat4 unbias = {
		 2.0f,  0.0f,  0.0f, 0.0f,
		 0.0f,  2.0f,  0.0f, 0.0f,
		 0.0f,  0.0f,  2.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f,
	};

	// final model matrix and full matrix stack
	a3mat4 viewProjectionMat = activeCamera->viewProjectionMat;
	a3mat4 modelViewProjectionMat = viewProjectionMat;
	a3mat4 modelMat = a3mat4_identity;
	a3mat4 modelViewProjectionBiasMat_other, viewProjectionBiasMat_other = activeShadowCaster->viewProjectionMat;
	a3mat4 projectionBiasMat = activeCamera->projectionMat, projectionBiasMat_inv = activeCamera->projectionMatInv;


	// init
	a3real4x4ConcatR(bias.m, viewProjectionBiasMat_other.m);
	modelViewProjectionBiasMat_other = viewProjectionBiasMat_other;

	a3real4x4Product(projectionBiasMat.m, bias.m, activeCamera->projectionMat.m);
	a3real4x4Product(projectionBiasMat_inv.m, activeCamera->projectionMatInv.m, unbias.m);


	//-------------------------------------------------------------------------
	// 0) PRE-SCENE PASS: shadow pass renders scene to depth-only
	//	- activate shadow pass framebuffer
	//	- draw scene
	//		- clear depth buffer
	//		- render shapes using appropriate shaders
	//		- capture depth

	// select shadow FBO
	currentPass = triangle_passShadow;
	currentWriteFBO = writeFBO[currentPass];
	a3framebufferActivate(currentWriteFBO);

	// clear
	glClear(GL_DEPTH_BUFFER_BIT);

	// draw objects inverted
	glCullFace(GL_FRONT);
	currentDemoProgram = demoState->prog_transform;
	a3shaderProgramActivate(currentDemoProgram->program);
	for (k = 0,
		currentSceneObject = demoState->planeObject, endSceneObject = demoState->teapotObject;
		currentSceneObject <= endSceneObject;
		++k, ++currentSceneObject)
		a3demo_drawModelSimple_activateModel(modelViewProjectionMat.m, activeShadowCaster->viewProjectionMat.m, currentSceneObject->modelMat.m, currentDemoProgram, drawable[k]);
	glCullFace(GL_BACK);

	//-------------------------------------------------------------------------
	// 1) SCENE PASS: render scene with desired shader
	//	- activate scene framebuffer
	//	- draw scene
	//		- clear buffers
	//		- render shapes using appropriate shaders
	//		- capture color and depth

	// select target framebuffer
	currentPass = triangle_passScene;
	currentWriteFBO = writeFBO[currentPass];


	switch (pipeline)
	{
		// shading with MRT
	case triangle_forward:
		// target scene framebuffer
		a3demo_setSceneState(currentWriteFBO, demoState->displaySkybox);
		break;

		// shading with MRT FBO
	//case triangle_fbo:
	//	// target scene framebuffer
	//	currentWriteFBO = writeFBO[pipeline];
	//	a3demo_setSceneState(currentWriteFBO, demoState->displaySkybox);
	//	break;
	}

	// optional stencil test before drawing objects
	a3real4x4SetScale(modelMat.m, a3real_four);
	if (demoState->stencilTest)
		a3demo_drawStencilTest(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, demoState->prog_drawColorUnif, demoState->draw_sphere);


	//// copy temp light data
	//for (k = 0, pointLight = demoState->forwardPointLight;
	//	k < demoState->forwardLightCount;
	//	++k, ++pointLight)
	//{
	//	lightSz[k] = pointLight->radius;
	//	lightSzInvSq[k] = pointLight->radiusInvSq;
	//	lightPos[k] = pointLight->viewPos;
	//	lightCol[k] = pointLight->color;
	//}

		// select program based on settings
	currentDemoProgram = renderProgram[pipeline][render];
	a3shaderProgramActivate(currentDemoProgram->program);



	// send shared data: 
	//	- projection matrix
	//	- light data
	//	- activate shared textures including atlases if using
	//	- shared animation data
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP, 1, activeCamera->projectionMat.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP_inv, 1, activeCamera->projectionMatInv.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uPB, 1, projectionBiasMat.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uPB_inv, 1, projectionBiasMat_inv.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
	a3shaderUniformSendDouble(a3unif_single, currentDemoProgram->uTime, 1, &demoState->renderTimer->totalTime);
	a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, skyblue);
	a3textureActivate(demoState->tex_ramp_dm, a3tex_unit04);
	a3textureActivate(demoState->tex_ramp_sm, a3tex_unit05);

	// select pipeline algorithm
	glDisable(GL_BLEND);

	switch (pipeline)
	{
		// scene pass using forward pipeline
	case triangle_forward: 
	{
		// activate shadow map and other relevant textures
		currentReadFBO = demoState->fbo_shadow_d32;
		a3framebufferBindDepthTexture(currentReadFBO, a3tex_unit06);
		a3textureActivate(demoState->tex_earth_dm, a3tex_unit07);

		// send more common uniforms
		a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uLightCt, 1, &demoState->forwardLightCount);
		a3shaderUniformBufferActivate(demoState->ubo_transformStack_model, 0);
		a3shaderUniformBufferActivate(demoState->ubo_pointLight, 4);

		// individual object requirements: 
		//	- modelviewprojection
		//	- modelview
		//	- modelview for normals
		//	- per-object animation data
		for (currentSceneObject = demoState->planeObject, endSceneObject = demoState->teapotObject,
			j = (a3ui32)(currentSceneObject - demoState->sceneObject), k = 0;
			currentSceneObject <= endSceneObject;
			++j, ++k, ++currentSceneObject)
		{
			// send data and draw
			a3textureActivate(texture_dm[k], a3tex_unit00);
			a3textureActivate(texture_sm[k], a3tex_unit01);
			a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, &j);
			a3vertexDrawableActivateAndRender(drawable[k]);
		}
	}	break;
		// end forward scene pass
	}

	// stop using stencil
	if (demoState->stencilTest)
		glDisable(GL_STENCIL_TEST);

	//-------------------------------------------------------------------------
	// COMPOSITE PASS
	//	- activate composite framebuffer
	//	- composite scene layers

	currentPass = triangle_passComposite;
	currentWriteFBO = writeFBO[currentPass];
	a3framebufferActivate(currentWriteFBO);

	// composite skybox
	currentDemoProgram = demoState->displaySkybox ? demoState->prog_drawTexture : demoState->prog_drawColorUnif;
	a3demo_drawModelTexturedColored_invertModel(modelViewProjectionMat.m, viewProjectionMat.m, demoState->skyboxObject->modelMat.m, a3mat4_identity.m, currentDemoProgram, demoState->draw_skybox, demoState->tex_skybox_clouds, grey);
	a3demo_enableCompositeBlending();

	// draw textured quad with previous pass image on it
	// repeat as necessary to complete composite
	currentDrawable = demoState->draw_unitquad;
	a3vertexDrawableActivate(currentDrawable);



	switch (pipeline)
	{
	case triangle_forward:
		// use simple texturing program
		currentDemoProgram = demoState->prog_drawTexture;
		a3shaderProgramActivate(currentDemoProgram->program);
		// scene (color)
		currentReadFBO = readFBO[currentPass][0];
		a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
		break;
	}
	// reset other uniforms
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, a3mat4_identity.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
	a3vertexDrawableRenderActive();

	//-------------------------------------------------------------------------
	// PREPARE FOR POST-PROCESSING
	//	- double buffer swap (if applicable)
	//	- ensure blending is disabled
	//	- re-activate FSQ drawable IF NEEDED (i.e. changed in previous step)
	glDisable(GL_BLEND);
	currentDrawable = demoState->draw_unitquad;
	a3vertexDrawableActivate(currentDrawable);

	//-------------------------------------------------------------------------
// POST-PROCESSING
//	- activate target framebuffer
//	- activate texture from previous framebuffer
//	- draw FSQ with processing program active

// bright passes: perform brighten or tone-mapping
//	-> simply sample from the previous pass result
// blur passes: perform 1D blur along specified axis
//	-> set pixel size to reciprocal of previous pass's write FBO
//	-> change axis per pass to reflect "horizontal" or "vertical"
// blend pass: blend all of the blurred images with the original scene
//	-> the blur passes iteratively flood the brightest areas over the 
//		image; composite with original scene to mix detail with light


	// bright-pass half-size
	currentDemoProgram = demoState->prog_drawTexture_brightPass;
	a3shaderProgramActivate(currentDemoProgram->program);

	currentPass = triangle_passBright_2;
	currentWriteFBO = writeFBO[currentPass];
	currentReadFBO = readFBO[currentPass][0];
	a3framebufferActivate(currentWriteFBO);
	a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
	a3vertexDrawableRenderActive();

	// blur half-size
	currentDemoProgram = demoState->prog_drawTexture_blurGaussian;
	a3shaderProgramActivate(currentDemoProgram->program);
	a3real2Set(pixelSize.v, a3recip((a3real)currentWriteFBO->frameWidth), a3recip((a3real)currentWriteFBO->frameHeight));
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uSize, 1, pixelSize.v);


	currentPass = triangle_passBlurH_2;
	currentWriteFBO = writeFBO[currentPass];
	currentReadFBO = readFBO[currentPass][0];
	a3framebufferActivate(currentWriteFBO);
	a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, sampleAxisH.v);
	a3vertexDrawableRenderActive();

	currentPass = triangle_passBlurV_2;
	currentWriteFBO = writeFBO[currentPass];
	currentReadFBO = readFBO[currentPass][0];
	a3framebufferActivate(currentWriteFBO);
	a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, sampleAxisV.v);
	a3vertexDrawableRenderActive();
	
	// bright-pass quarter-size
	currentDemoProgram = demoState->prog_drawTexture_brightPass;
	a3shaderProgramActivate(currentDemoProgram->program);

	currentPass = triangle_passBright_4;
	currentWriteFBO = writeFBO[currentPass];
	currentReadFBO = readFBO[currentPass][0];
	a3framebufferActivate(currentWriteFBO);
	a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
	a3vertexDrawableRenderActive();

	// blur quarter-size
	currentDemoProgram = demoState->prog_drawTexture_blurGaussian;
	a3shaderProgramActivate(currentDemoProgram->program);
	a3real2Set(pixelSize.v, a3recip((a3real)currentWriteFBO->frameWidth), a3recip((a3real)currentWriteFBO->frameHeight));
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uSize, 1, pixelSize.v);

	currentPass = triangle_passBlurH_4;
	currentWriteFBO = writeFBO[currentPass];
	currentReadFBO = readFBO[currentPass][0];
	a3framebufferActivate(currentWriteFBO);
	a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, sampleAxisH.v);
	a3vertexDrawableRenderActive();

	currentPass = triangle_passBlurV_4;
	currentWriteFBO = writeFBO[currentPass];
	currentReadFBO = readFBO[currentPass][0];
	a3framebufferActivate(currentWriteFBO);
	a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, sampleAxisV.v);
	a3vertexDrawableRenderActive();

	// bright-pass eighth-size
	currentDemoProgram = demoState->prog_drawTexture_brightPass;
	a3shaderProgramActivate(currentDemoProgram->program);

	currentPass = triangle_passBright_8;
	currentWriteFBO = writeFBO[currentPass];
	currentReadFBO = readFBO[currentPass][0];
	a3framebufferActivate(currentWriteFBO);
	a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
	a3vertexDrawableRenderActive();

	// blur eighth-size
	currentDemoProgram = demoState->prog_drawTexture_blurGaussian;
	a3shaderProgramActivate(currentDemoProgram->program);
	a3real2Set(pixelSize.v, a3recip((a3real)currentWriteFBO->frameWidth), a3recip((a3real)currentWriteFBO->frameHeight));
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uSize, 1, pixelSize.v);

	currentPass = triangle_passBlurH_8;
	currentWriteFBO = writeFBO[currentPass];
	currentReadFBO = readFBO[currentPass][0];
	a3framebufferActivate(currentWriteFBO);
	a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, sampleAxisH.v);
	a3vertexDrawableRenderActive();

	currentPass = triangle_passBlurV_8;
	currentWriteFBO = writeFBO[currentPass];
	currentReadFBO = readFBO[currentPass][0];
	a3framebufferActivate(currentWriteFBO);
	a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
	a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, sampleAxisV.v);
	a3vertexDrawableRenderActive();

	// bloom composite
	currentDemoProgram = demoState->prog_drawTexture_blendScreen4;
	a3shaderProgramActivate(currentDemoProgram->program);

	currentPass = triangle_passBlend;
	currentWriteFBO = writeFBO[currentPass];
	a3framebufferActivate(currentWriteFBO);
	for (i = 0, j = 4; i < j; ++i)
		a3framebufferBindColorTexture(readFBO[currentPass][i], a3tex_unit00 + i, 0);
	a3vertexDrawableRenderActive();

	//-------------------------------------------------------------------------
	// DISPLAY: final pass, perform and present final composite
	//	- finally draw to back buffer
	//	- select display texture(s)
	//	- activate final pass program
	//	- draw final FSQ

	// revert to back buffer and disable depth testing
	a3framebufferDeactivateSetViewport(a3fbo_depthDisable,
		-demoState->frameBorder, -demoState->frameBorder, demoState->frameWidth, demoState->frameHeight);

	// select framebuffer to display based on mode
	currentDisplayFBO = writeFBO[demoMode->pass];

	// select output to display
	switch (demoMode->pass)
	{
	case triangle_passShadow:
		a3framebufferBindDepthTexture(currentDisplayFBO, a3tex_unit00);
		break;
	case triangle_passScene:
		if (currentDisplayFBO->color && (!currentDisplayFBO->depthStencil || targetIndex < targetCount - 1))
			a3framebufferBindColorTexture(currentDisplayFBO, a3tex_unit00, targetIndex);
		else
			a3framebufferBindDepthTexture(currentDisplayFBO, a3tex_unit00);
		break;
	case triangle_passComposite:
	case triangle_passBright_2:
	case triangle_passBlurH_2:
	case triangle_passBlurV_2:
	case triangle_passBright_4:
	case triangle_passBlurH_4:
	case triangle_passBlurV_4:
	case triangle_passBright_8:
	case triangle_passBlurH_8:
	case triangle_passBlurV_8:
	case triangle_passBlend:
		a3framebufferBindColorTexture(currentDisplayFBO, a3tex_unit00, targetIndex);
		break;
	}

	// final display: activate desired final program and draw FSQ
	if (currentDisplayFBO)
	{
		// prepare for final draw
		currentDrawable = demoState->draw_unitquad;
		a3vertexDrawableActivate(currentDrawable);

		// determine if additional passes are required
		currentDemoProgram = displayProgram[display];
		a3shaderProgramActivate(currentDemoProgram->program);

		switch (demoMode->display)
		{
			// most basic option: simply display texture
		case triangle_displayTexture:
			break;
		}

		// done
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, a3mat4_identity.mm);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
		a3shaderUniformSendDouble(a3unif_single, currentDemoProgram->uTime, 1, &demoState->renderTimer->totalTime);
		a3vertexDrawableRenderActive();
	}

	//-------------------------------------------------------------------------
	// OVERLAYS: done after FSQ so they appear over everything else
	//	- disable depth testing
	//	- draw overlays appropriately

	// enable alpha
	a3demo_enableCompositeBlending();

	// scene overlays
	if (demoState->displayGrid || demoState->displayTangentBases || demoState->displayWireframe)
	{
		// activate scene FBO and clear color; reuse depth
		currentWriteFBO = demoState->fbo_scene_c16d24s8_mrt;
		a3framebufferActivate(currentWriteFBO);
		glDisable(GL_STENCIL_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw grid aligned to world
		if (demoState->displayGrid)
		{
			a3demo_drawModelSolidColor(modelViewProjectionMat.m, viewProjectionMat.m, demoState->gridTransform.m, demoState->prog_drawColorUnif, demoState->draw_grid, demoState->gridColor.v);
		}
		if (demoState->displayTangentBases || demoState->displayWireframe)
		{
			const a3i32 flag[1] = { demoState->displayTangentBases * 1 + demoState->displayWireframe * 2 };
			const a3f32 size[1] = { 0.1f };

			currentDemoProgram = demoState->prog_drawOverlays_tangents_wireframe;
			a3shaderProgramActivate(currentDemoProgram->program);

			// projection matrix
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP, 1, activeCamera->projectionMat.mm);
			// scene object matrix stack
			a3shaderUniformBufferActivate(demoState->ubo_transformStack_model, 0);
			// wireframe color
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, orange);
			// tangent basis size
			a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uSize, 1, size);
			// overlay flag
			a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uFlag, 1, flag);

			// draw objects again
			for (currentSceneObject = demoState->planeObject, endSceneObject = demoState->teapotObject,
				j = (a3ui32)(currentSceneObject - demoState->sceneObject), k = 0;
				currentSceneObject <= endSceneObject;
				++j, ++k, ++currentSceneObject)
			{
				a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, &j);
				a3vertexDrawableActivateAndRender(drawable[k]);
			}

			// display color target with scene overlays
			a3framebufferDeactivateSetViewport(a3fbo_depthDisable,
				-demoState->frameBorder, -demoState->frameBorder, demoState->frameWidth, demoState->frameHeight);
			currentDrawable = demoState->draw_unitquad;
			currentDemoProgram = demoState->prog_drawTexture;
			a3vertexDrawableActivate(currentDrawable);
			a3shaderProgramActivate(currentDemoProgram->program);
			a3framebufferBindColorTexture(currentWriteFBO, a3tex_unit00, 0);
			a3vertexDrawableRenderActive();

		}

		// hidden volumes
		if (demoState->displayHiddenVolumes && demoMode->pass != triangle_passShadow)
		{

			glCullFace(GL_FRONT);

			// draw light volumes
			currentDemoProgram = demoState->prog_drawColorUnif;
			a3shaderProgramActivate(currentDemoProgram->program);
			a3vertexDrawableActivate(demoState->draw_pointlight);
			for (k = 0; k < demoState->forwardLightCount; ++k)
			{
				a3real4x4SetScale(modelMat.m, (a3real)0.0075 * demoState->forwardPointLight[k].radius);
				modelMat.v3 = demoState->forwardPointLight[k].worldPos;
				a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, demoState->forwardPointLight[k].color.v);
				a3demo_drawModelSimple(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, currentDemoProgram);
			}

			a3vertexDrawableActivate(demoState->draw_skybox);
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, orange);
			for (k = 0; k < demoStateMaxCount_cameraObject; ++k)
			{
				a3real4x4SetScale(modelMat.m, (a3real)0.0075);
				a3real4x4ConcatR(demoState->cameraObject[k].modelMat.m, modelMat.m);
				a3demo_drawModelSimple(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, currentDemoProgram);
			}

			glCullFace(GL_BACK);
		}


	}

		// superimpose axes
	// draw coordinate axes in front of everything
		currentDemoProgram = demoState->prog_drawColorAttrib;
		a3shaderProgramActivate(currentDemoProgram->program);
		a3vertexDrawableActivate(demoState->draw_axes);

		// center of world from current viewer
		// also draw other viewer/viewer-like object in scene
		if (demoState->displayWorldAxes && demoMode->pass != triangle_passShadow)
		{
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, viewProjectionMat.mm);
			a3vertexDrawableRenderActive();
		}


		// individual objects
		if (demoState->displayObjectAxes && demoMode->pass != triangle_passShadow)
		{
			a3_DemoSceneObject const* axesObjects[] = {
				demoState->planeObject, demoState->sphereObject, demoState->cylinderObject, demoState->torusObject, demoState->triangleObject, demoState->teapotObject,
				demoState->mainLightObject, demoState->mainCameraObject,
				0
			}, ** axesObjectsItr = axesObjects;
			for (currentSceneObject = *axesObjectsItr,
				endSceneObject = demoState->displayHiddenVolumes ? 0 : demoState->mainLightObject;
				currentSceneObject != endSceneObject;
				currentSceneObject = *(++axesObjectsItr))
				a3demo_drawModelSimple(modelViewProjectionMat.m, viewProjectionMat.m, currentSceneObject->modelMat.m, currentDemoProgram);
		}


}

//-----------------------------------------------------------------------------
