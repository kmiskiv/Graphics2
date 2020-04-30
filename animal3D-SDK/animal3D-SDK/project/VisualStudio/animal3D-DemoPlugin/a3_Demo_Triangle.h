#pragma once

/* A demo mode for my final project
based off of the other demo mode files
Kayla Miskiv, Spring 2020 */


#ifndef __ANIMAL3D_DEMO_TRIANGLE_H
#define __ANIMAL3D_DEMO_TRIANGLE_H




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
typedef enum a3_Demo_Triangle_InterpolationModeName	a3_Demo_Triangle_InterpolationModeName;
#endif	// __cplusplus

//-----------------------------------------------------------------------------


	// program to use for rendering
enum a3_Demo_Triangle_RenderProgramName
{
	triangle_renderSolid,		// solid color
	triangle_renderTexture,		// texture sample
	triangle_renderLambert,		// Lambert shading
	triangle_renderPhong,		// Phong shading

	triangle_render_max
};


// program to use for display
enum a3_Demo_Triangle_DisplayProgramName
{
	triangle_displayTexture,
	triangle_displayTextureManipColor,
	triangle_displayTextureManipTexcoord,

	triangle_display_max
};


// active camera names
enum a3_Demo_Triangle_ActiveCameraName
{
	triangle_cameraSceneViewer,	// scene viewing camera

	triangle_camera_max
};

// shading pipeline names
enum a3_Demo_Triangle_PipelineName
{
	triangle_back,				// on-screen rendering with back buffer
	triangle_fbo,				// off-screen rendering with MRT FBO

	triangle_pipeline_max
};

enum a3_Demo_Triangle_TargetName
{
	triangle_back_composite = 0,	// final composite color
	triangle_target_back_max,

	triangle_fbo_composite = 0,	// final composite color
	triangle_fbo_position,		// position attribute
	triangle_fbo_normal,			// normal attribute
	triangle_fbo_texcoord,		// texcoord attribute
	triangle_fbo_diffuseTex,		// diffuse texture sample
	triangle_fbo_specularTex,	// specular texture sample
	triangle_fbo_diffuseLight,	// diffuse light total
	triangle_fbo_specularLight,	// specular light total
	triangle_fbo_fragdepth,		// fragment depth
	triangle_target_fbo_max,
};

//-----------------------------------------------------------------------------

	// demo mode for basic shading
struct a3_Demo_Triangle
{
	a3_Demo_Triangle_RenderProgramName render;
	a3_Demo_Triangle_DisplayProgramName display;
	a3_Demo_Triangle_ActiveCameraName activeCamera;

	a3_Demo_Triangle_PipelineName pipeline;
	a3_Demo_Triangle_TargetName targetIndex[triangle_pipeline_max], targetCount[triangle_pipeline_max];
};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMO_TRIANGLE_H









