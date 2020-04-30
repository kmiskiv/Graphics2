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


	a3byte const* targetText_back[triangle_target_back_max] =
	{
		"Color buffer",

	};

	a3byte const* const* targetText_fbo[triangle_target_fbo_max] =
	{
		"Color target 0: FINAL SCENE COLOR",
		"Color target 1: Attrib data: view position",
		"Color target 2: Attrib data: view normal",
		"Color target 3: Attrib data: atlas texcoord",
		"Color target 4: Texture: diffuse map",
		"Color target 5: Texture: specular map",
		"Color target 6: Lighting: diffuse total",
		"Color target 7: Lighting: specular total",
		"Depth buffer",
	};

	a3byte const* const* targetText[triangle_pipeline_max] =
	{
		targetText_back,
		targetText_fbo,
	};

	//forward pipeline names
	a3byte const* renderProgramName[triangle_render_max] =
	{
		"Solid color",
		"Texturing",
		"Lambert shading",
		"Phong shading",
	};


	a3byte const* displayProgramName[triangle_display_max] =
	{
			"Texture",
		"Color manipulation",
		"Texcoord manipulation",

	};


	// text color
	a3vec4 const col = { a3real_half, a3real_zero, a3real_half, a3real_one };

	// pipeline and target
	a3_Demo_Triangle_RenderProgramName const render = demoMode->render;
	a3_Demo_Triangle_DisplayProgramName const display = demoMode->display;
	a3_Demo_Triangle_PipelineName const pipeline = demoMode->pipeline;
	a3_Demo_Triangle_TargetName const targetIndex = demoMode->targetIndex[pipeline];
	a3_Demo_Triangle_TargetName const targetCount = demoMode->targetCount[pipeline];

	//demoModes
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Pipeline (%u / %u) ('k' | 'l'): %s", pipeline + 1, shading_pipeline_max, pipelineText[pipeline]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"        Target (%u / %u) ('t' | 'y'): %s", targetIndex + 1, targetCount, targetText[pipeline][targetIndex]);

	// lighting modes
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Rendering mode (%u / %u) ('g' | 'h'): %s", render + 1, shading_render_max, renderProgramName[render]);

	// specific modes
	if (pipeline == shading_fbo)
		a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"    Display mode (%u / %u) ('v' | 'b'): %s", display + 1, shading_display_max, displayProgramName[display]);

}
	//-----------------------------------------------------------------------------

void a3shading_render(a3_DemoState const* demoState, a3_Demo_Triangle const* demoMode)
{



}

