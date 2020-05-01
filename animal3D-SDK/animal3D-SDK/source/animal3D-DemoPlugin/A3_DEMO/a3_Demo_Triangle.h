
/* A demo mode for my final project
based off of the other demo mode files
Kayla Miskiv, Spring 2020 */


#ifndef __ANIMAL3D_DEMO_TRIANGLE_H
#define __ANIMAL3D_DEMO_TRIANGLE_H


#include "animal3D/animal3D.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef struct a3_Demo_Triangle						a3_Demo_Triangle;
typedef enum a3_Demo_Triangle_RenderProgramName		a3_Demo_Triangle_RenderProgramName;
typedef enum a3_Demo_Triangle_DisplayProgramName		a3_Demo_Triangle_DisplayProgramName;
typedef enum a3_Demo_Triangle_ActiveCameraName			a3_Demo_Triangle_ActiveCameraName;
typedef enum a3_Demo_Triangle_PipelineName				a3_Demo_Triangle_PipelineName;
typedef enum a3_Demo_Triangle_PassName					a3_Demo_Triangle_PassName;
typedef enum a3_Demo_Triangle_TargetName				a3_Demo_Triangle_TargetName;
typedef enum a3_Demo_Triangles_InterpolationModeName	a3_Demo_Triangle_InterpolationModeName;
#endif	// __cplusplus

//-----------------------------------------------------------------------------


	// program to use for rendering
enum a3_Demo_Triangle_RenderProgramName
{
	triangle_renderPhong,		// Phong shading

	triangle_render_max
};


// program to use for display
enum a3_Demo_Triangle_DisplayProgramName
{
	triangle_displayTexture,

	triangle_display_max
};


// active camera names
enum a3_Demo_Triangle_ActiveCameraName
{
	triangle_cameraSceneViewer,	// scene viewing camera
	triangle_cameraShadowLight,

	triangle_camera_max
};

// shading pipeline names
enum a3_Demo_Triangle_PipelineName
{
	triangle_forward,				
	//triangle_fbo,				// off-screen rendering with MRT FBO

	triangle_pipeline_max
};

enum a3_Demo_Triangle_TargetName
{
	triangle_shadow_fragdepth = 0,
	triangle_target_shadow_max,

	triangle_scene_finalcolor = 0,
	triangle_scene_position,
	triangle_scene_normal,
	triangle_scene_texcoord,
	triangle_scene_shadowcoord,
	triangle_scene_shadowtest,
	triangle_scene_diffuseLight,
	triangle_scene_specularLight,
	triangle_scene_fragdepth,
	triangle_target_scene_max,

	triangle_composite_finalcolor = 0,
	triangle_target_composite_max,
	
	triangle_bright_finalcolor = 0,
	triangle_bright_luminance,
	triangle_target_bright_max,

	triangle_blur_finalcolor = 0,
	triangle_target_blur_max,

	triangle_display_finalcolor = 0,
	triangle_target_display_max,
};

enum a3_Demo_Triangle_PassName
{
	triangle_passShadow,			// capture shadow map

	triangle_passScene,			// render scene objects

	triangle_passComposite,		// composite for post-processing stage

	triangle_passBright_2,		// bright pass for bloom (half screen size)
	triangle_passBlurH_2,			// horizontal blur for bloom (half screen size)
	triangle_passBlurV_2,			// horizontal blur for bloom (half screen size)
	triangle_passBright_4,		// bright pass for bloom (quarter screen size)
	triangle_passBlurH_4,			// horizontal blur for bloom (quarter screen size)
	triangle_passBlurV_4,			// horizontal blur for bloom (quarter screen size)
	triangle_passBright_8,		// bright pass for bloom (eighth screen size)
	triangle_passBlurH_8,			// horizontal blur for bloom (eighth screen size)
	triangle_passBlurV_8,			// horizontal blur for bloom (eighth screen size)
	triangle_passBlend,			// bloom composite pass

	triangle_pass_max
};

//-----------------------------------------------------------------------------

	// demo mode for basic shading
struct a3_Demo_Triangle
{
	a3_Demo_Triangle_RenderProgramName render;
	a3_Demo_Triangle_DisplayProgramName display;
	a3_Demo_Triangle_ActiveCameraName activeCamera;

	a3_Demo_Triangle_PassName pass; 

	a3_Demo_Triangle_PipelineName pipeline;
	a3_Demo_Triangle_TargetName targetIndex[triangle_pass_max], targetCount[triangle_pass_max];
};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMO_TRIANGLE_H









