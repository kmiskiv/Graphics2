/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_DemoState_loading.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     LOADING in this file.                    ***
	****************************************************
*/

//-----------------------------------------------------------------------------

// uncomment this to link extension library (if available)
//#define A3_USER_ENABLE_EXTENSION

// **WARNING: FOR TESTING/COMPARISON ONLY, DO NOT USE IN DELIVERABLE BUILDS**
// uncomment this to allow shader decoding (if available)
#define A3_USER_ENABLE_SHADER_DECODING


//-----------------------------------------------------------------------------

#ifdef A3_USER_ENABLE_SHADER_DECODING
// override shader loading function name before including
#define a3shaderCreateFromFileList a3shaderCreateFromFileListEncoded
#endif	// A3_USER_ENABLE_SHADER_DECODING


#ifdef _WIN32
#ifdef A3_USER_ENABLE_EXTENSION
// force link extension lib
#pragma comment(lib,"animal3D-A3DX.lib")
#endif	// A3_USER_ENABLE_EXTENSION
#ifdef A3_USER_ENABLE_SHADER_DECODING
// add lib for shader decoding
#pragma comment(lib,"animal3D-UtilityLib.lib")
#endif	// A3_USER_ENABLE_SHADER_DECODING
#endif	// _WIN32


// define resource directories
#define A3_DEMO_RES_DIR	"../../../../resource/"
#define A3_DEMO_GLSL	A3_DEMO_RES_DIR"glsl/"
#define A3_DEMO_TEX		A3_DEMO_RES_DIR"tex/"
#define A3_DEMO_OBJ		A3_DEMO_RES_DIR"obj/"

// define resource subdirectories
#define A3_DEMO_VS		A3_DEMO_GLSL"4x/vs/"
#define A3_DEMO_TS		A3_DEMO_GLSL"4x/ts/"
#define A3_DEMO_GS		A3_DEMO_GLSL"4x/gs/"
#define A3_DEMO_FS		A3_DEMO_GLSL"4x/fs/"
#define A3_DEMO_CS		A3_DEMO_GLSL"4x/cs/"


//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"

#include <stdio.h>


//-----------------------------------------------------------------------------
// GENERAL UTILITIES

inline a3real4x4r a3demo_setAtlasTransform_internal(a3real4x4p m_out,
	const a3ui16 atlasWidth, const a3ui16 atlasHeight,
	const a3ui16 subTexturePosX, const a3ui16 subTexturePosY,
	const a3ui16 subTextureWidth, const a3ui16 subTextureHeight,
	const a3ui16 subTextureBorderPadding, const a3ui16 subTextureAdditionalPadding)
{
	a3real4x4SetIdentity(m_out);
	m_out[0][0] = (a3real)(subTextureWidth) / (a3real)(atlasWidth);
	m_out[1][1] = (a3real)(subTextureHeight) / (a3real)(atlasHeight);
	m_out[3][0] = (a3real)(subTexturePosX + subTextureBorderPadding + subTextureAdditionalPadding) / (a3real)(atlasWidth);
	m_out[3][1] = (a3real)(subTexturePosY + subTextureBorderPadding + subTextureAdditionalPadding) / (a3real)(atlasHeight);
	return m_out;
}


// initialize dummy drawable
inline void a3demo_initDummyDrawable_internal(a3_DemoState *demoState)
{
	// dummy drawable for point drawing: copy any of the existing ones, 
	//	set vertex count to 1 and primitive to points (0x0000)
	// DO NOT RELEASE THIS DRAWABLE; it is a managed stand-in!!!
	*demoState->dummyDrawable = *demoState->draw_grid;
	demoState->dummyDrawable->primitive = 0;
	demoState->dummyDrawable->count = 1;
}


//-----------------------------------------------------------------------------
// uniform helpers

#define a3demo_getUniformLocation(demoProgram, handleName, getLocFunc) (demoProgram->handleName = getLocFunc(demoProgram->program, #handleName))
#define a3demo_getUniformLocationValid(demoProgram, handleName, getLocFunc) if (a3demo_getUniformLocation(demoProgram, handleName, getLocFunc) >= 0)
#define a3demo_setUniformDefault(demoProgram, handleName, sendFunc, type, value) \
	a3demo_getUniformLocationValid(demoProgram, handleName, a3shaderUniformGetLocation) \
		sendFunc(type, demoProgram->handleName, 1, value)
#define a3demo_setUniformDefaultMat(demoProgram, handleName, sendFunc, type, value) \
	a3demo_getUniformLocationValid(demoProgram, handleName, a3shaderUniformGetLocation) \
		sendFunc(type, 0, demoProgram->handleName, 1, value)
#define a3demo_setUniformDefaultBlock(demoProgram, handleName, value) \
	a3demo_getUniformLocationValid(demoProgram, handleName, a3shaderUniformBlockGetLocation) \
		a3shaderUniformBlockBind(demoProgram->program, demoProgram->handleName, value)

#define a3demo_setUniformDefaultFloat(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_single, value)
#define a3demo_setUniformDefaultVec2(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_vec2, value)
#define a3demo_setUniformDefaultVec3(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_vec3, value)
#define a3demo_setUniformDefaultVec4(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendFloat, a3unif_vec4, value)
#define a3demo_setUniformDefaultDouble(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_single, value)
#define a3demo_setUniformDefaultDVec2(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_vec2, value)
#define a3demo_setUniformDefaultDVec3(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_vec3, value)
#define a3demo_setUniformDefaultDVec4(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendDouble, a3unif_vec4, value)
#define a3demo_setUniformDefaultInteger(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_single, value)
#define a3demo_setUniformDefaultIVec2(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_vec2, value)
#define a3demo_setUniformDefaultIVec3(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_vec3, value)
#define a3demo_setUniformDefaultIVec4(demoProgram, handleName, value) a3demo_setUniformDefault(demoProgram, handleName, a3shaderUniformSendInt, a3unif_vec4, value)
#define a3demo_setUniformDefaultMat2(demoProgram, handleName) a3demo_setUniformDefaultMat(demoProgram, handleName, a3shaderUniformSendFloatMat, a3unif_mat2, a3mat2_identity.mm)
#define a3demo_setUniformDefaultMat3(demoProgram, handleName) a3demo_setUniformDefaultMat(demoProgram, handleName, a3shaderUniformSendFloatMat, a3unif_mat3, a3mat3_identity.mm)
#define a3demo_setUniformDefaultMat4(demoProgram, handleName) a3demo_setUniformDefaultMat(demoProgram, handleName, a3shaderUniformSendFloatMat, a3unif_mat4, a3mat4_identity.mm)


//-----------------------------------------------------------------------------
// LOADING

// utility to load geometry
void a3demo_loadGeometry(a3_DemoState *demoState)
{
	// tmp descriptor for loaded model
	typedef struct a3_TAG_DEMOSTATELOADEDMODEL {
		const a3byte *filePath;
		const a3real *transform;
		a3_ModelLoaderFlag flag;
	} a3_DemoStateLoadedModel;

	// static model transformations
	static const a3mat4 downscale20x = {
		+0.05f, 0.0f, 0.0f, 0.0f,
		0.0f, +0.05f, 0.0f, 0.0f,
		0.0f, 0.0f, +0.05f, 0.0f,
		0.0f, 0.0f, 0.0f, +1.0f,
	};

	// pointer to shared vbo/ibo
	a3_VertexBuffer *vbo_ibo;
	a3_VertexArrayDescriptor *vao;
	a3_VertexDrawable *currentDrawable;
	a3ui32 sharedVertexStorage = 0, sharedIndexStorage = 0;
	a3ui32 numVerts = 0;
	a3ui32 i;
//	a3ui32 j;


	// file streaming (if requested)
	a3_FileStream fileStream[1] = { 0 };
	const a3byte *const geometryStream = "./data/geom_data_gpro_coursebase.dat";

	// geometry data
	a3_GeometryData displayShapesData[5] = { 0 };
	a3_GeometryData hiddenShapesData[3] = { 0 };
	a3_GeometryData proceduralShapesData[5] = { 0 };
	a3_GeometryData loadedModelsData[1] = { 0 };
	a3_GeometryData morphModelsData[5] = { 0 };
	const a3ui32 displayShapesCount = sizeof(displayShapesData) / sizeof(a3_GeometryData);
	const a3ui32 hiddenShapesCount = sizeof(hiddenShapesData) / sizeof(a3_GeometryData);
	const a3ui32 proceduralShapesCount = sizeof(proceduralShapesData) / sizeof(a3_GeometryData);
	const a3ui32 loadedModelsCount = sizeof(loadedModelsData) / sizeof(a3_GeometryData);
	const a3ui32 morphModelsCount = sizeof(morphModelsData) / sizeof(a3_GeometryData);

	// common index format
	a3_IndexFormatDescriptor sceneCommonIndexFormat[1] = { 0 };
	a3ui32 bufferOffset, *const bufferOffsetPtr = &bufferOffset;


	// procedural scene objects
	// attempt to load stream if requested
	if (demoState->streaming && a3fileStreamOpenRead(fileStream, geometryStream))
	{
		// read from stream

		// static display objects
		for (i = 0; i < displayShapesCount; ++i)
			a3fileStreamReadObject(fileStream, displayShapesData + i, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// hidden volume objects
		for (i = 0; i < hiddenShapesCount; ++i)
			a3fileStreamReadObject(fileStream, hiddenShapesData + i, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// procedurally-generated objects
		for (i = 0; i < proceduralShapesCount; ++i)
			a3fileStreamReadObject(fileStream, proceduralShapesData + i, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// loaded model objects
		for (i = 0; i < loadedModelsCount; ++i)
			a3fileStreamReadObject(fileStream, loadedModelsData + i, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// morph model objects
		for (i = 0; i < morphModelsCount; ++i)
			a3fileStreamReadObject(fileStream, morphModelsData + i, (a3_FileStreamReadFunc)a3geometryLoadDataBinary);

		// done
		a3fileStreamClose(fileStream);
	}
	// not streaming or stream doesn't exist
	else if (!demoState->streaming || a3fileStreamOpenWrite(fileStream, geometryStream))
	{
		// create new data
		a3_ProceduralGeometryDescriptor displayShapes[5] = { a3geomShape_none };
		a3_ProceduralGeometryDescriptor hiddenShapes[3] = { a3geomShape_none };
		a3_ProceduralGeometryDescriptor proceduralShapes[5] = { a3geomShape_none };
		const a3_DemoStateLoadedModel loadedShapes[1] = {
			{ A3_DEMO_OBJ"teapot/teapot.obj", downscale20x.mm, a3model_calculateVertexTangents },
		};
		const a3_DemoStateLoadedModel morphShapes[5] = {
			{ A3_DEMO_OBJ"teapot/morph/teapot_base.obj", downscale20x.mm, a3model_calculateVertexTangents },
			{ A3_DEMO_OBJ"teapot/morph/teapot_scale.obj", downscale20x.mm, a3model_calculateVertexTangents },
			{ A3_DEMO_OBJ"teapot/morph/teapot_scale_x.obj", downscale20x.mm, a3model_calculateVertexTangents },
			{ A3_DEMO_OBJ"teapot/morph/teapot_scale_y.obj", downscale20x.mm, a3model_calculateVertexTangents },
			{ A3_DEMO_OBJ"teapot/morph/teapot_scale_z.obj", downscale20x.mm, a3model_calculateVertexTangents },
		};

		const a3ubyte lightVolumeSlices = 8, lightVolumeStacks = 6;
		const a3real lightVolumeRadius = a3real_one;

		// static scene procedural objects
		//	(axes, grid, skybox, unit quad, triangle)
		a3proceduralCreateDescriptorAxes(displayShapes + 0, a3geomFlag_wireframe, 0.0f, 1);
		a3proceduralCreateDescriptorPlane(displayShapes + 1, a3geomFlag_wireframe, a3geomAxis_default, 20.0f, 20.0f, 20, 20);
		a3proceduralCreateDescriptorBox(displayShapes + 2, a3geomFlag_texcoords, 100.0f, 100.0f, 100.0f, 1, 1, 1);
		a3proceduralCreateDescriptorPlane(displayShapes + 3, a3geomFlag_texcoords, a3geomAxis_default, 2.0f, 2.0f, 1, 1);
		a3proceduralCreateDescriptorTriangle(displayShapes + 4, a3geomFlag_wireframe, a3geomAxis_x, 5.0f, 2.5f);
		for (i = 0; i < displayShapesCount; ++i)
		{
			a3proceduralGenerateGeometryData(displayShapesData + i, displayShapes + i, 0);
			a3fileStreamWriteObject(fileStream, displayShapesData + i, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
		}

		// hidden volumes and shapes
		//	(light volumes)
		a3proceduralCreateDescriptorSphere(hiddenShapes + 0, a3geomFlag_vanilla, a3geomAxis_default, 
			lightVolumeRadius * a3trigFaceToPointRatio(a3real_threesixty, a3real_oneeighty, lightVolumeSlices, lightVolumeStacks),
			lightVolumeSlices, lightVolumeStacks);
		// (skeletal shapes)
		a3proceduralCreateDescriptorDiamond(hiddenShapes + 1, a3geomFlag_wireframe, a3geomAxis_default, 0.1f, 0.2f, 4, 1);
		a3proceduralCreateDescriptorCone(hiddenShapes + 2, a3geomFlag_wireframe, a3geomAxis_default, 0.1f, 1.0f, 4, 1, 1);
		for (i = 0; i < hiddenShapesCount; ++i)
		{
			a3proceduralGenerateGeometryData(hiddenShapesData + i, hiddenShapes + i, 0);
			a3fileStreamWriteObject(fileStream, hiddenShapesData + i, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
		}

		// other procedurally-generated objects
		a3proceduralCreateDescriptorPlane(proceduralShapes + 0, a3geomFlag_tangents, a3geomAxis_default, 24.0f, 24.0f, 12, 12);
		a3proceduralCreateDescriptorSphere(proceduralShapes + 1, a3geomFlag_tangents, a3geomAxis_default, 1.0f, 32, 24);
		a3proceduralCreateDescriptorCylinder(proceduralShapes + 2, a3geomFlag_tangents, a3geomAxis_x, 1.0f, 2.0f, 32, 1, 1);
		a3proceduralCreateDescriptorTorus(proceduralShapes + 3, a3geomFlag_tangents, a3geomAxis_x, 1.0f, 0.25f, 32, 24);
		a3proceduralCreateDescriptorTriangle(proceduralShapes + 4, a3geomFlag_tangents, a3geomAxis_x, 5.0f, 2.5f);
		for (i = 0; i < proceduralShapesCount; ++i)
		{
			a3proceduralGenerateGeometryData(proceduralShapesData + i, proceduralShapes + i, 0);
			a3fileStreamWriteObject(fileStream, proceduralShapesData + i, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
		}

		// objects loaded from mesh files
		for (i = 0; i < loadedModelsCount; ++i)
		{
			a3modelLoadOBJ(loadedModelsData + i, loadedShapes[i].filePath, loadedShapes[i].flag, loadedShapes[i].transform);
			a3fileStreamWriteObject(fileStream, loadedModelsData + i, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
		}

		// morph targets loaded from mesh files
		for (i = 0; i < morphModelsCount; ++i)
		{
			a3modelLoadOBJ(morphModelsData + i, morphShapes[i].filePath, morphShapes[i].flag, morphShapes[i].transform);
			a3fileStreamWriteObject(fileStream, morphShapes + i, (a3_FileStreamWriteFunc)a3geometrySaveDataBinary);
		}

		// done
		a3fileStreamClose(fileStream);
	}


	// GPU data upload process: 
	//	- determine storage requirements
	//	- allocate buffer
	//	- create vertex arrays using unique formats
	//	- create drawable and upload data

	// get storage size
	sharedVertexStorage = numVerts = 0;
	for (i = 0; i < displayShapesCount; ++i)
	{
		sharedVertexStorage += a3geometryGetVertexBufferSize(displayShapesData + i);
		numVerts += displayShapesData[i].numVertices;
	}
	for (i = 0; i < hiddenShapesCount; ++i)
	{
		sharedVertexStorage += a3geometryGetVertexBufferSize(hiddenShapesData + i);
		numVerts += hiddenShapesData[i].numVertices;
	}
	for (i = 0; i < proceduralShapesCount; ++i)
	{
		sharedVertexStorage += a3geometryGetVertexBufferSize(proceduralShapesData + i);
		numVerts += proceduralShapesData[i].numVertices;
	}
	for (i = 0; i < loadedModelsCount; ++i)
	{
		sharedVertexStorage += a3geometryGetVertexBufferSize(loadedModelsData + i);
		numVerts += loadedModelsData[i].numVertices;
	}


	// common index format required for shapes that share vertex formats
	a3geometryCreateIndexFormat(sceneCommonIndexFormat, numVerts);
	sharedIndexStorage = 0;
	for (i = 0; i < displayShapesCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, displayShapesData[i].numIndices);
	for (i = 0; i < hiddenShapesCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, hiddenShapesData[i].numIndices);
	for (i = 0; i < proceduralShapesCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, proceduralShapesData[i].numIndices);
	for (i = 0; i < loadedModelsCount; ++i)
		sharedIndexStorage += a3indexFormatGetStorageSpaceRequired(sceneCommonIndexFormat, loadedModelsData[i].numIndices);

	// create shared buffer
	vbo_ibo = demoState->vbo_staticSceneObjectDrawBuffer;
	a3bufferCreateSplit(vbo_ibo, "vbo/ibo:scene", a3buffer_vertex, sharedVertexStorage, sharedIndexStorage, 0, 0);
	sharedVertexStorage = 0;

	// create vertex formats and drawables
	// grid: position attribute only
	// overlay objects are also just position
	vao = demoState->vao_position;
	a3geometryGenerateVertexArray(vao, "vao:pos", displayShapesData + 1, vbo_ibo, sharedVertexStorage);
	currentDrawable = demoState->draw_grid;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 1, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_pointlight;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, hiddenShapesData + 0, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_skeletal_joint;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, hiddenShapesData + 1, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_skeletal_bone;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, hiddenShapesData + 2, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);

	// axes: position and color
	vao = demoState->vao_position_color;
	a3geometryGenerateVertexArray(vao, "vao:pos+col", displayShapesData + 0, vbo_ibo, sharedVertexStorage);
	currentDrawable = demoState->draw_axes;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 0, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);

	// skybox, unit quad: position and texture coordinates
	vao = demoState->vao_position_texcoord;
	a3geometryGenerateVertexArray(vao, "vao:pos+tex", displayShapesData + 2, vbo_ibo, sharedVertexStorage);
	currentDrawable = demoState->draw_skybox;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 2, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_unitquad;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 3, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_triangle;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, displayShapesData + 4, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);




	// models: position, texture coordinates and normals
//	vao = demoState->vao_position_texcoord_normal;
//	a3geometryGenerateVertexArray(vao, "vao:pos+tex+nrm", proceduralShapesData + 0, vbo_ibo, sharedVertexStorage);
	vao = demoState->vao_tangentbasis;
	a3geometryGenerateVertexArray(vao, "vao:tangentbasis", proceduralShapesData + 0, vbo_ibo, sharedVertexStorage);
	currentDrawable = demoState->draw_plane;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 0, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_sphere;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 1, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_cylinder;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 2, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_torus;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, proceduralShapesData + 3, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	currentDrawable = demoState->draw_teapot;
	sharedVertexStorage += a3geometryGenerateDrawable(currentDrawable, loadedModelsData + 0, vao, vbo_ibo, sceneCommonIndexFormat, 0, 0);
	

	// release data when done
	for (i = 0; i < displayShapesCount; ++i)
		a3geometryReleaseData(displayShapesData + i);
	for (i = 0; i < hiddenShapesCount; ++i)
		a3geometryReleaseData(hiddenShapesData + i);
	for (i = 0; i < proceduralShapesCount; ++i)
		a3geometryReleaseData(proceduralShapesData + i);
	for (i = 0; i < loadedModelsCount; ++i)
		a3geometryReleaseData(loadedModelsData + i);
	for (i = 0; i < morphModelsCount; ++i)
		a3geometryReleaseData(morphModelsData + i);


	// dummy
	a3demo_initDummyDrawable_internal(demoState);
}


// utility to load shaders
void a3demo_loadShaders(a3_DemoState *demoState)
{
	// direct to demo programs
	a3_DemoStateShaderProgram *currentDemoProg;
	a3i32 flag;
	a3ui32 i;

	// maximum uniform buffer size
	const a3ui32 uBlockSzMax = a3shaderUniformBlockMaxSize();

	// some default uniform values
	const a3f32 defaultFloat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const a3f64 defaultDouble[] = { 0.0, 0.0, 0.0, 1.0 };
	const a3i32 defaultInt[] = { 0, 0, 0, 1 };
	const a3i32 defaultTexUnits[] = {
		a3tex_unit00, a3tex_unit01, a3tex_unit02, a3tex_unit03,
		a3tex_unit04, a3tex_unit05, a3tex_unit06, a3tex_unit07,
		a3tex_unit08, a3tex_unit09, a3tex_unit10, a3tex_unit11,
		a3tex_unit12, a3tex_unit13, a3tex_unit14, a3tex_unit15
	};


	// list of all unique shaders
	// this is a good idea to avoid multi-loading 
	//	those that are shared between programs
	union {
		struct {
			// vertex shaders
			// base
			a3_DemoStateShader
				passthru_vs[1],
				passthru_transform_vs[1],
				passColor_transform_vs[1],
				passthru_transform_instanced_vs[1],
				passColor_transform_instanced_vs[1];
			// 02-shading
			a3_DemoStateShader
				passTexcoord_transform_vs[1],
				passLightingData_transform_vs[1];
			// 04-multipass
			a3_DemoStateShader
				passLightingData_shadowCoord_transform_vs[1];
			// 06-deferred
			a3_DemoStateShader
				passAtlasTexcoord_transform_vs[1],
				passLightingData_transform_bias_vs[1],
				passBiasedClipCoord_transform_instanced_vs[1];
			// 07-curves
			a3_DemoStateShader
				passTangentBasis_transform_instanced_vs[1];
			// 07-keyframes
			a3_DemoStateShader
				passColor_hierarchy_transform_instanced_vs[1],
				passTangentBasis_transform_instanced_morph_vs[1];
			// final
			a3_DemoStateShader
				finalvertexshader_vs[1]; 

			// geometry shaders
			// 07-curves
			a3_DemoStateShader
				drawCurveSegment_gs[1],
				drawOverlays_tangents_wireframe_gs[1];

			// fragment shaders
			// base
			a3_DemoStateShader
				drawColorUnif_fs[1],
				drawColorAttrib_fs[1];
			// 02-shading
			a3_DemoStateShader
				drawTexture_fs[1],
				drawLambert_multi_fs[1],
				drawPhong_multi_fs[1],
				drawNonphoto_multi_fs[1];
			// 03-framebuffer
			a3_DemoStateShader
				drawTexture_mrt_fs[1],
				drawTexture_colorManip_fs[1],
				drawTexture_coordManip_fs[1],
				drawLambert_multi_mrt_fs[1],
				drawPhong_multi_mrt_fs[1],
				drawNonphoto_multi_mrt_fs[1];
			// 04-multipass
			a3_DemoStateShader
				drawTexture_outline_fs[1],
				drawPhong_multi_shadow_mrt_fs[1];
			// 05-bloom
			a3_DemoStateShader
				drawTexture_brightPass_fs[1],
				drawTexture_blurGaussian_fs[1],
				drawTexture_blendScreen4_fs[1];
			// 06-deferred
			a3_DemoStateShader
				drawLightingData_fs[1],
				drawPhong_multi_deferred_fs[1],
				drawPhongVolume_fs[1],
				drawPhongComposite_fs[1];
			// 07-curves
			a3_DemoStateShader
				drawPhong_multi_forward_mrt_fs[1];
			//final 
			a3_DemoStateShader
				finalfragshader_fs[1]; 



		};
	} shaderList = {
		{
			// ****REMINDER: 'Encoded' shaders are available for proof-of-concept
			//	testing ONLY!  Insert /e before file names.
			// DO NOT SUBMIT WORK USING ENCODED SHADERS OR YOU WILL GET ZERO!!!

			// vs
			// base
			{ { { 0 },	"shdr-vs:passthru",					a3shader_vertex  ,	1,{ A3_DEMO_VS"passthru_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:passthru-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"passthru_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-col-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"passColor_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:passthru-trans-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"passthru_transform_instanced_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-col-trans-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"passColor_transform_instanced_vs4x.glsl" } } },
			// 02-shading
			{ { { 0 },	"shdr-vs:pass-tex-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"02-shading/passTexcoord_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-light-trans",			a3shader_vertex  ,	1,{ A3_DEMO_VS"02-shading/passLightingData_transform_vs4x.glsl" } } },
			// 04-multipass
			{ { { 0 },	"shdr-vs:pass-light-shadow-trans",	a3shader_vertex  ,	1,{ A3_DEMO_VS"04-multipass/passLightingData_shadowCoord_transform_vs4x.glsl" } } },
			// 06-deferred
			{ { { 0 },	"shdr-vs:pass-atlas-tex-trans",		a3shader_vertex  ,	1,{ A3_DEMO_VS"06-deferred/e/passAtlasTexcoord_transform_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-light-trans-bias",	a3shader_vertex  ,	1,{ A3_DEMO_VS"06-deferred/e/passLightingData_transform_bias_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-biasedclip-inst",		a3shader_vertex  ,	1,{ A3_DEMO_VS"06-deferred/e/passBiasedClipCoord_transform_instanced_vs4x.glsl" } } },
			// 07-curves
			{ { { 0 },	"shdr-vs:pass-tangent-trans-inst",	a3shader_vertex  ,	1,{ A3_DEMO_VS"07-curves/passTangentBasis_transform_instanced_vs4x.glsl" } } },
			// 07-keyframes
			{ { { 0 },	"shdr-vs:pass-col-hierarchy-t-i",	a3shader_vertex  ,	1,{ A3_DEMO_VS"07-keyframes/passColor_hierarchy_transform_instanced_vs4x.glsl" } } },
			{ { { 0 },	"shdr-vs:pass-tb-trans-morph",		a3shader_vertex  ,	1,{ A3_DEMO_VS"07-keyframes/passTangentBasis_transform_instanced_morph_vs4x.glsl" } } },
			//final	
			{ { { 0 }, "shdr-vs:finalvertexshader", a3shader_vertex, 1, { A3_DEMO_VS"final/finalvertexshader_vs4x.glsl" } } },

			// gs
			// 07-curves
			{ { { 0 },	"shdr-gs:draw-curve-segment",		a3shader_geometry,	1,{ A3_DEMO_GS"07-curves/e/drawCurveSegment_gs4x.glsl" } } },
			{ { { 0 },	"shdr-gs:draw-overlays-tb-wire",	a3shader_geometry,	1,{ A3_DEMO_GS"07-curves/e/drawOverlays_tangents_wireframe_gs4x.glsl" } } },

			// fs
			// base
			{ { { 0 },	"shdr-fs:draw-col-unif",			a3shader_fragment,	1,{ A3_DEMO_FS"drawColorUnif_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-col-attr",			a3shader_fragment,	1,{ A3_DEMO_FS"drawColorAttrib_fs4x.glsl" } } },
			// 02-shading
			{ { { 0 },	"shdr-fs:draw-tex",					a3shader_fragment,	1,{ A3_DEMO_FS"02-shading/drawTexture_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Lambert-multi",		a3shader_fragment,	1,{ A3_DEMO_FS"02-shading/drawLambert_multi_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Phong-multi",			a3shader_fragment,	1,{ A3_DEMO_FS"02-shading/drawPhong_multi_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-nonphoto-multi",		a3shader_fragment,	1,{ A3_DEMO_FS"02-shading/drawNonphoto_multi_fs4x.glsl" } } },
			// 03-framebuffer
			{ { { 0 },	"shdr-fs:draw-tex-mrt",				a3shader_fragment,	1,{ A3_DEMO_FS"03-framebuffer/drawTexture_mrt_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-tex-colormanip",		a3shader_fragment,	1,{ A3_DEMO_FS"03-framebuffer/drawTexture_colorManip_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-tex-coordmanip",		a3shader_fragment,	1,{ A3_DEMO_FS"03-framebuffer/drawTexture_coordManip_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Lambert-multi-mrt",	a3shader_fragment,	1,{ A3_DEMO_FS"03-framebuffer/drawLambert_multi_mrt_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Phong-multi-mrt",		a3shader_fragment,	1,{ A3_DEMO_FS"03-framebuffer/drawPhong_multi_mrt_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-nonphoto-multi-mrt",	a3shader_fragment,	1,{ A3_DEMO_FS"03-framebuffer/e/drawNonphoto_multi_mrt_fs4x.glsl" } } },
			// 04-multipass
			{ { { 0 },	"shdr-fs:draw-tex-outline",			a3shader_fragment,	1,{ A3_DEMO_FS"04-multipass/drawTexture_outline_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Phong-multi-shadow",	a3shader_fragment,	1,{ A3_DEMO_FS"04-multipass/drawPhong_multi_shadow_mrt_fs4x.glsl" } } },
			// 05-bloom
			{ { { 0 },	"shdr-fs:draw-tex-bright",			a3shader_fragment,	1,{ A3_DEMO_FS"05-bloom/e/drawTexture_brightPass_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-tex-blur",			a3shader_fragment,	1,{ A3_DEMO_FS"05-bloom/e/drawTexture_blurGaussian_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-tex-blend4",			a3shader_fragment,	1,{ A3_DEMO_FS"05-bloom/e/drawTexture_blendScreen4_fs4x.glsl" } } },
			// 06-deferred
			{ { { 0 },	"shdr-fs:draw-lightingdata",		a3shader_fragment,	1,{ A3_DEMO_FS"06-deferred/e/drawLightingData_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Phong-multi-def",		a3shader_fragment,	1,{ A3_DEMO_FS"06-deferred/e/drawPhong_multi_deferred_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Phong-volume",		a3shader_fragment,	1,{ A3_DEMO_FS"06-deferred/e/drawPhongVolume_fs4x.glsl" } } },
			{ { { 0 },	"shdr-fs:draw-Phong-composite",		a3shader_fragment,	1,{ A3_DEMO_FS"06-deferred/e/drawPhongComposite_fs4x.glsl" } } },
			// 07-curves
			{ { { 0 },	"shdr-fs:draw-Phong-mul-fwd-mrt",	a3shader_fragment,	1,{ A3_DEMO_FS"07-curves/drawPhong_multi_forward_mrt_fs4x.glsl" } } },
			//final
			{ { { 0 }, "shdr-fs:finalfragshader",			a3shader_fragment,  1,{ A3_DEMO_FS"final/finalgragshader_fs4x.glsl" } } },


		}
	};
	a3_DemoStateShader *const shaderListPtr = (a3_DemoStateShader *)(&shaderList), *shaderPtr;
	const a3ui32 numUniqueShaders = sizeof(shaderList) / sizeof(a3_DemoStateShader);


	printf("\n\n---------------- LOAD SHADERS STARTED  ---------------- \n");


	// load unique shaders: 
	//	- load file contents
	//	- create and compile shader object
	//	- release file contents
	for (i = 0; i < numUniqueShaders; ++i)
	{
		shaderPtr = shaderListPtr + i;
		flag = a3shaderCreateFromFileList(shaderPtr->shader,
			shaderPtr->shaderName, shaderPtr->shaderType,
			shaderPtr->filePath, shaderPtr->srcCount);
		if (flag == 0)
			printf("\n ^^^^ SHADER %u '%s' FAILED TO COMPILE \n\n", i, shaderPtr->shader->handle->name);
	}


	// setup programs: 
	//	- create program object
	//	- attach shader objects

	// base programs: 
	// transform-only program
	currentDemoProg = demoState->prog_transform;
	a3shaderProgramCreate(currentDemoProg->program, "prog:transform");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_vs->shader);
	// transform-only program with instancing
	currentDemoProg = demoState->prog_transform_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:transform-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_instanced_vs->shader);
	// uniform color program
	currentDemoProg = demoState->prog_drawColorUnif;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-unif");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorUnif_fs->shader);
	// color attrib program
	currentDemoProg = demoState->prog_drawColorAttrib;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-attr");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passColor_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// uniform color program with instancing
	currentDemoProg = demoState->prog_drawColorUnif_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-unif-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorUnif_fs->shader);
	// color attrib program with instancing
	currentDemoProg = demoState->prog_drawColorAttrib_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-col-attr-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passColor_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);

	// 02-shading programs: 
	// texturing program
	currentDemoProg = demoState->prog_drawTexture;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_fs->shader);
	// Lambert shading program
	currentDemoProg = demoState->prog_drawLambert_multi;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Lambert-multi");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passLightingData_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawLambert_multi_fs->shader);
	// Phong shading program
	currentDemoProg = demoState->prog_drawPhong_multi;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-multi");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passLightingData_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_multi_fs->shader);
	// nonphotorealistic shading program
	currentDemoProg = demoState->prog_drawNonphoto_multi;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-nonphoto-multi");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passLightingData_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawNonphoto_multi_fs->shader);

	// 03-framebuffer programs: 
	// texturing program with MRT
	currentDemoProg = demoState->prog_drawTexture_mrt;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex-mrt");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_mrt_fs->shader);
	// Lambert shading program with MRT
	currentDemoProg = demoState->prog_drawLambert_multi_mrt;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Lambert-multi-mrt");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passLightingData_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawLambert_multi_mrt_fs->shader);
	// Phong shading program with MRT
	currentDemoProg = demoState->prog_drawPhong_multi_mrt;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-multi-mrt");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passLightingData_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_multi_mrt_fs->shader);
	// nonphotorealistic shading program with MRT
	currentDemoProg = demoState->prog_drawNonphoto_multi_mrt;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-nonphoto-multi-mrt");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passLightingData_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawNonphoto_multi_mrt_fs->shader);
	// texturing with color manipulation
	currentDemoProg = demoState->prog_drawTexture_colorManip;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex-colormanip");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_colorManip_fs->shader);
	// texturing with texcoord manipulation
	currentDemoProg = demoState->prog_drawTexture_coordManip;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex-coordmanip");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_coordManip_fs->shader);

	// 04-multipass programs: 
	// Phong shading with shadow mapping and MRT
	currentDemoProg = demoState->prog_drawPhong_multi_shadow_mrt;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-multi-shadow");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passLightingData_shadowCoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_multi_shadow_mrt_fs->shader);
	// texturing program with outlines
	currentDemoProg = demoState->prog_drawTexture_outline;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex-outline");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_outline_fs->shader);

	// 05-bloom programs: 
	// texturing with bright-pass or tone-mapping
	currentDemoProg = demoState->prog_drawTexture_brightPass;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex-bright");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_brightPass_fs->shader);
	// texturing with Gaussian blurring
	currentDemoProg = demoState->prog_drawTexture_blurGaussian;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex-blur");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_blurGaussian_fs->shader);
	// texturing with bloom composition
	currentDemoProg = demoState->prog_drawTexture_blendScreen4;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-tex-blend4");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawTexture_blendScreen4_fs->shader);

	// 06-deferred programs: 
	// draw lighting data as g-buffers
	currentDemoProg = demoState->prog_drawLightingData;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-lightingdata");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passLightingData_transform_bias_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawLightingData_fs->shader);
	// draw Phong shading deferred
	currentDemoProg = demoState->prog_drawPhong_multi_deferred;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-multi-def");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passAtlasTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_multi_deferred_fs->shader);
	// draw Phong light volume
	currentDemoProg = demoState->prog_drawPhongVolume_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-volume-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passBiasedClipCoord_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhongVolume_fs->shader);
	// draw composited Phong shading model
	currentDemoProg = demoState->prog_drawPhongComposite;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-composite");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passAtlasTexcoord_transform_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhongComposite_fs->shader);

	// 07-curves programs: 
	// draw Phong forward MRT
	currentDemoProg = demoState->prog_drawPhong_multi_forward_mrt;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-mul-fwd-mrt");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_multi_forward_mrt_fs->shader);
	// draw overlays (tangents & wireframe)
	currentDemoProg = demoState->prog_drawOverlays_tangents_wireframe;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-overlays-tb-wire");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawOverlays_tangents_wireframe_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// draw curve segment
	currentDemoProg = demoState->prog_drawCurveSegment;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-curve-segment");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passthru_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawCurveSegment_gs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	
	// 07-keyframes programs: 
	// draw Phong forward MRT with morphing
	currentDemoProg = demoState->prog_drawPhong_multi_forward_mrt_morph;
//	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-Phong-mul-morph");
//	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_instanced_morph_vs->shader);
//	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_multi_forward_mrt_fs->shader);
	// draw overlays with morphing
	currentDemoProg = demoState->prog_drawOverlays_tb_wf_morph;
//	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-overlays-tb-wf-morph");
//	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passTangentBasis_transform_instanced_morph_vs->shader);
//	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawOverlays_tangents_wireframe_gs->shader);
//	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);
	// draw colorized hierarchy
	currentDemoProg = demoState->prog_drawColorizedHierarchy_instanced;
	a3shaderProgramCreate(currentDemoProg->program, "prog:draw-color-hierarchy-inst");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.passColor_hierarchy_transform_instanced_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawColorAttrib_fs->shader);

	//final triangle program
	currentDemoProg = demoState->prog_drawFinal; 
	a3shaderProgramCreate(currentDemoProg->program, "prog:drawFinal");
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.finalvertexshader_vs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.finalfragshader_fs->shader);
	a3shaderProgramAttachShader(currentDemoProg->program, shaderList.drawPhong_multi_shadow_mrt_fs->shader);



	// activate a primitive for validation
	// makes sure the specified geometry can draw using programs
	// good idea to activate the drawable with the most attributes
	a3vertexDrawableActivate(demoState->draw_axes);

	// link and validate all programs
	for (i = 0; i < demoStateMaxCount_shaderProgram; ++i)
	{
		currentDemoProg = demoState->shaderProgram + i;
		flag = a3shaderProgramLink(currentDemoProg->program);
		if (flag == 0)
			printf("\n ^^^^ PROGRAM %u '%s' FAILED TO LINK \n\n", i, currentDemoProg->program->handle->name);

		flag = a3shaderProgramValidate(currentDemoProg->program);
		if (flag == 0)
			printf("\n ^^^^ PROGRAM %u '%s' FAILED TO VALIDATE \n\n", i, currentDemoProg->program->handle->name);
	}

	// if linking fails, contingency plan goes here
	// otherwise, release shaders
	for (i = 0; i < numUniqueShaders; ++i)
	{
		shaderPtr = shaderListPtr + i;
		a3shaderRelease(shaderPtr->shader);
	}


	// prepare uniforms algorithmically instead of manually for all programs
	// get uniform and uniform block locations and set default values for all 
	//	programs that have a uniform that will either never change or is
	//	consistent for all programs
	for (i = 0; i < demoStateMaxCount_shaderProgram; ++i)
	{
		// activate program
		currentDemoProg = demoState->shaderProgram + i;
		a3shaderProgramActivate(currentDemoProg->program);

		// common VS
		a3demo_setUniformDefaultMat4(currentDemoProg, uMVP);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMV);
		a3demo_setUniformDefaultMat4(currentDemoProg, uP);
		a3demo_setUniformDefaultMat4(currentDemoProg, uP_inv);
		a3demo_setUniformDefaultMat4(currentDemoProg, uPB);
		a3demo_setUniformDefaultMat4(currentDemoProg, uPB_inv);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMV_nrm);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMVPB);
		a3demo_setUniformDefaultMat4(currentDemoProg, uMVPB_other);
		a3demo_setUniformDefaultMat4(currentDemoProg, uAtlas);
		
		// common FS
		a3demo_setUniformDefaultInteger(currentDemoProg, uLightCt, defaultInt);
		a3demo_setUniformDefaultFloat(currentDemoProg, uLightSz, defaultFloat);
		a3demo_setUniformDefaultFloat(currentDemoProg, uLightSzInvSq, defaultFloat);
		a3demo_setUniformDefaultVec4(currentDemoProg, uLightPos, a3vec4_w.v);
		a3demo_setUniformDefaultVec4(currentDemoProg, uLightCol, a3vec4_one.v);
		a3demo_setUniformDefaultVec4(currentDemoProg, uColor, a3vec4_one.v);

		// common texture
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_dm, defaultTexUnits + 0);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_sm, defaultTexUnits + 1);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_nm, defaultTexUnits + 2);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_hm, defaultTexUnits + 3);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_dm_ramp, defaultTexUnits + 4);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_sm_ramp, defaultTexUnits + 5);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_shadow, defaultTexUnits + 6);
		a3demo_setUniformDefaultInteger(currentDemoProg, uTex_proj, defaultTexUnits + 7);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage00, defaultTexUnits + 0);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage01, defaultTexUnits + 1);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage02, defaultTexUnits + 2);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage03, defaultTexUnits + 3);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage04, defaultTexUnits + 4);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage05, defaultTexUnits + 5);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage06, defaultTexUnits + 6);
		a3demo_setUniformDefaultInteger(currentDemoProg, uImage07, defaultTexUnits + 7);

		// common general
		a3demo_setUniformDefaultInteger(currentDemoProg, uIndex, defaultInt);
		a3demo_setUniformDefaultInteger(currentDemoProg, uCount, defaultInt);
		a3demo_setUniformDefaultInteger(currentDemoProg, uFlag, defaultInt);
		a3demo_setUniformDefaultDouble(currentDemoProg, uAxis, defaultDouble);
		a3demo_setUniformDefaultDouble(currentDemoProg, uSize, defaultDouble);
		a3demo_setUniformDefaultDouble(currentDemoProg, uTime, defaultDouble);

		// transformation uniform blocks
		a3demo_setUniformDefaultBlock(currentDemoProg, ubTransformStack, 0);
		a3demo_setUniformDefaultBlock(currentDemoProg, ubTransformMVP, 0);
		a3demo_setUniformDefaultBlock(currentDemoProg, ubTransformMVPB, 1);

		// lighting uniform blocks
		a3demo_setUniformDefaultBlock(currentDemoProg, ubPointLight, 4);

		// animation uniform blocks
		a3demo_setUniformDefaultBlock(currentDemoProg, ubHierarchy, 4);
		a3demo_setUniformDefaultBlock(currentDemoProg, ubCurveWaypoint, 4);
	}


	// set up lighting uniform buffers
	for (i = 0; i < demoStateMaxCount_lightVolumeBlock; ++i)
	{
		a3bufferCreate(demoState->ubo_transformMVP_light + i, "ubo:transform-mvp", a3buffer_uniform, a3index_countMaxShort, 0);
		a3bufferCreate(demoState->ubo_transformMVPB_light + i, "ubo:transform-mvpb", a3buffer_uniform, a3index_countMaxShort, 0);
		a3bufferCreate(demoState->ubo_pointLight + i, "ubo:pointlight", a3buffer_uniform, a3index_countMaxShort, 0);
	}

	// set up transform uniform buffers
	for (i = 0; i < demoStateMaxCount_transformUniformBuffer; ++i)
	{
		a3bufferCreate(demoState->ubo_transformStack_model + i, "ubo:transform-stack", a3buffer_uniform, a3index_countMaxShort, 0);
	}

	// set up miscellaneous uniform buffers
	a3bufferCreate(demoState->ubo_transformLMVP_joint, "ubo:transform-lmvp-joint", a3buffer_uniform, a3index_countMaxShort, 0);
	a3bufferCreate(demoState->ubo_transformLMVP_bone, "ubo:transform-lmvp-bone", a3buffer_uniform, a3index_countMaxShort, 0);
	a3bufferCreate(demoState->ubo_hierarchy, "ubo:hierarchy", a3buffer_uniform, a3index_countMaxShort, 0);
	a3bufferCreate(demoState->ubo_curveWaypoint, "ubo:curvewaypoint", a3buffer_uniform, a3index_countMaxShort, 0);



	printf("\n\n---------------- LOAD SHADERS FINISHED ---------------- \n");

	//done
	a3shaderProgramDeactivate();
	a3vertexDrawableDeactivate();
}


// utility to load textures
void a3demo_loadTextures(a3_DemoState* demoState)
{
	// utilities
	const a3ui16 atlasSceneWidth = 2048, atlasSceneHeight = 2048, atlasSceneBorderPad = 8, atlasSceneAdditionalPad = 8;
	
	// indexing
	a3_Texture* tex;
	a3ui32 i;

	// structure for texture loading
	typedef struct a3_TAG_DEMOSTATETEXTURE {
		a3_Texture* texture;
		a3byte textureName[32];
		const a3byte* filePath;
	} a3_DemoStateTexture;

	// texture objects
	union {
		struct {
			a3_DemoStateTexture texSkyClouds[1];
			a3_DemoStateTexture texSkyWater[1];
			a3_DemoStateTexture texAtlasDM[1];
			a3_DemoStateTexture texAtlasSM[1];
			a3_DemoStateTexture texEarthDM[1];
			a3_DemoStateTexture texEarthSM[1];
			a3_DemoStateTexture texMarsDM[1];
			a3_DemoStateTexture texMarsSM[1];
			a3_DemoStateTexture texStoneDM[1];
			a3_DemoStateTexture texRampDM[1];
			a3_DemoStateTexture texRampSM[1];
			a3_DemoStateTexture texChecker[1];
		};
	} textureList = {
		{
			{ demoState->tex_skybox_clouds,	"tex:sky-clouds",	"../../../../resource/tex/bg/sky_clouds.png" },
			{ demoState->tex_skybox_water,	"tex:sky-water",	"../../../../resource/tex/bg/sky_water.png" },
			{ demoState->tex_atlas_dm,		"tex:atlas-dm",		"../../../../resource/tex/atlas/atlas_scene_dm.png" },
			{ demoState->tex_atlas_sm,		"tex:atlas-sm",		"../../../../resource/tex/atlas/atlas_scene_sm.png" },
			{ demoState->tex_earth_dm,		"tex:earth-dm",		"../../../../resource/tex/earth/2k/earth_dm_2k.png" },
			{ demoState->tex_earth_sm,		"tex:earth-sm",		"../../../../resource/tex/earth/2k/earth_sm_2k.png" },
			{ demoState->tex_mars_dm,		"tex:mars-dm",		"../../../../resource/tex/mars/1k/mars_1k_dm.png" },
			{ demoState->tex_mars_sm,		"tex:mars-sm",		"../../../../resource/tex/mars/1k/mars_1k_sm.png" },
			{ demoState->tex_stone_dm,		"tex:stone-dm",		"../../../../resource/tex/stone/stone_dm.png" },
			{ demoState->tex_ramp_dm,		"tex:ramp-dm",		"../../../../resource/tex/sprite/celRamp_dm.png" },
			{ demoState->tex_ramp_sm,		"tex:ramp-sm",		"../../../../resource/tex/sprite/celRamp_sm.png" },
			{ demoState->tex_checker,		"tex:checker",		"../../../../resource/tex/sprite/checker.png" },
		}
	};
	const a3ui32 numTextures = sizeof(textureList) / sizeof(a3_DemoStateTexture);
	a3_DemoStateTexture* const textureListPtr = (a3_DemoStateTexture*)(&textureList), * texturePtr;

	// load all textures
	for (i = 0; i < numTextures; ++i)
	{
		texturePtr = textureListPtr + i;
		a3textureCreateFromFile(texturePtr->texture, texturePtr->textureName, texturePtr->filePath);
		a3textureActivate(texturePtr->texture, a3tex_unit00);
		a3textureDefaultSettings();
	}

	// change settings on a per-texture or per-type basis
	tex = demoState->texture;
	// skyboxes
	for (i = 0; i < 2; ++i, ++tex)
	{
		a3textureActivate(tex, a3tex_unit00);
		a3textureChangeFilterMode(a3tex_filterLinear);	// linear pixel blending
	}
	// atlases
	for (i = 0; i < 2; ++i, ++tex)
	{
		a3textureActivate(tex, a3tex_unit00);
		a3textureChangeFilterMode(a3tex_filterLinear);
	}
	// stone and planets
	for (i = 0; i < 5; ++i, ++tex)
	{
		a3textureActivate(tex, a3tex_unit00);
		a3textureChangeFilterMode(a3tex_filterLinear);
	}
	// ramps
	for (i = 0; i < 2; ++i, ++tex)
	{
		a3textureActivate(tex, a3tex_unit00);
		a3textureChangeRepeatMode(a3tex_repeatClamp, a3tex_repeatClamp);	// clamp both axes
	}


	// set up texture atlas transforms
	a3demo_setAtlasTransform_internal(demoState->atlas_stone->m, atlasSceneWidth, atlasSceneHeight,
		1600, 0, 256, 256, atlasSceneBorderPad, atlasSceneAdditionalPad);
	a3demo_setAtlasTransform_internal(demoState->atlas_earth->m, atlasSceneWidth, atlasSceneHeight,
		0, 0, 1024, 512, atlasSceneBorderPad, atlasSceneAdditionalPad);
	a3demo_setAtlasTransform_internal(demoState->atlas_mars->m, atlasSceneWidth, atlasSceneHeight,
		0, 544, 1024, 512, atlasSceneBorderPad, atlasSceneAdditionalPad);
	a3demo_setAtlasTransform_internal(demoState->atlas_checker->m, atlasSceneWidth, atlasSceneHeight,
		1888, 0, 128, 128, atlasSceneBorderPad, atlasSceneAdditionalPad);


	// done
	a3textureDeactivate(a3tex_unit00);
}


// utility to load framebuffers
void a3demo_loadFramebuffers(a3_DemoState* demoState)
{
	// create framebuffers and change their texture settings if need be
	a3_Framebuffer* fbo;
	a3ui32 i, j;

	// frame sizes
	const a3ui16 frameWidth1 = demoState->frameWidth, frameHeight1 = demoState->frameHeight;
	const a3ui16 frameWidth2 = frameWidth1 / 2, frameHeight2 = frameHeight1 / 2;
	const a3ui16 frameWidth4 = frameWidth2 / 2, frameHeight4 = frameHeight2 / 2;
	const a3ui16 frameWidth8 = frameWidth4 / 2, frameHeight8 = frameHeight4 / 2;

	// storage precision and targets
	const a3_FramebufferColorType colorType_scene = a3fbo_colorRGBA16;
	const a3_FramebufferDepthType depthType_scene = a3fbo_depth24_stencil8;
	const a3ui32 targets_scene = 8;

	const a3_FramebufferDepthType depthType_shadow = a3fbo_depth32;
	const a3ui16 shadowMapSz = 2048;

	const a3_FramebufferColorType colorType_composite = a3fbo_colorRGBA8;//a3fbo_colorRGBA16;
	const a3ui32 targets_composite = 8;

	const a3_FramebufferColorType colorType_post = colorType_composite;
	const a3ui32 targets_post = 2;


	// initialize framebuffers: 
	//	-> scene, with or without MRT (determine your needs), add depth
	fbo = demoState->fbo_scene_c16d24s8_mrt;
	a3framebufferCreate(fbo, "fbo:scene",
		targets_scene, colorType_scene, depthType_scene,
		frameWidth1, frameHeight1);

	//	-> shadow map, depth only
	fbo = demoState->fbo_shadow_d32;
	a3framebufferCreate(fbo, "fbo:shadow",
		0, a3fbo_colorDisable, depthType_shadow,
		shadowMapSz, shadowMapSz);

	for (i = 0; i < 3; ++i)
	{
		//	-> compositing, color only
		fbo = demoState->fbo_composite_c16 + i;
		a3framebufferCreate(fbo, "fbo:composite",
			targets_composite, colorType_composite, a3fbo_depthDisable,
			frameWidth1, frameHeight1);
		
		//	-> post-processing, color only
		fbo = demoState->fbo_post_c16_2fr + i;
		a3framebufferCreate(fbo, "fbo:post-half",
			targets_post, colorType_post, a3fbo_depthDisable,
			frameWidth2, frameHeight2);
		fbo = demoState->fbo_post_c16_4fr + i;
		a3framebufferCreate(fbo, "fbo:post-quarter",
			targets_post, colorType_post, a3fbo_depthDisable,
			frameWidth4, frameHeight4);
		fbo = demoState->fbo_post_c16_8fr + i;
		a3framebufferCreate(fbo, "fbo:post-eighth",
			targets_post, colorType_post, a3fbo_depthDisable,
			frameWidth8, frameHeight8);
	}


	// change texture settings for all framebuffers
	for (i = 0, fbo = demoState->framebuffer;
		i < demoStateMaxCount_framebuffer;
		++i, ++fbo)
	{
		// color, if applicable
		for (j = 0; j < fbo->color; ++j)
		{
			a3framebufferBindColorTexture(fbo, a3tex_unit00, j);
			a3textureChangeRepeatMode(a3tex_repeatClamp, a3tex_repeatClamp);
			a3textureChangeFilterMode(a3tex_filterLinear);
		}

		// depth, if applicable
		if (fbo->depthStencil)
		{
			a3framebufferBindDepthTexture(fbo, a3tex_unit00);
			a3textureChangeRepeatMode(a3tex_repeatClamp, a3tex_repeatClamp);
			a3textureChangeFilterMode(a3tex_filterLinear);
		}
	}


	// deactivate texture
	a3textureDeactivate(a3tex_unit00);
}


// utility to load animation
void a3demo_loadAnimation(a3_DemoState* demoState)
{
	// general counters
	a3ui32 j, p;

	// object pointers
	a3_Hierarchy *hierarchy;
	a3_HierarchyState *hierarchyState;
	a3_HierarchyPoseGroup *hierarchyPoseGroup;
	a3_HierarchyNodePose *hierarchyNodePose;
	a3_HierarchyPoseFlag *hierarchyPoseFlag;

	a3_FileStream fileStream[1] = { 0 };
	const a3byte* const geometryStream = "./data/anim_data_gpro_base.dat";

	// stream animation assets
	if (demoState->streaming && a3fileStreamOpenRead(fileStream, geometryStream))
	{
		// load hierarchy assets
		hierarchy = demoState->hierarchy_skel;
		a3hierarchyLoadBinary(hierarchy, fileStream);

		// done
		a3fileStreamClose(fileStream);
	}
	// not streaming or stream doesn't exist
	else if (!demoState->streaming || a3fileStreamOpenWrite(fileStream, geometryStream))
	{
		// manually set up a skeleton
		// first is the hierarchy: the general non-spatial relationship between bones
		const a3ui32 jointCount = 32;

		// indices of joints, their parents and branching joints
		a3ui32 jointIndex = 0;
		a3i32 jointParentIndex = -1;
		a3i32 rootJointIndex, upperSpineJointIndex, clavicleJointIndex, pelvisJointIndex;

		// initialize hierarchy
		hierarchy = demoState->hierarchy_skel;
		a3hierarchyCreate(hierarchy, jointCount, 0);

		// set up joint relationships
		jointParentIndex = rootJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:root");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:spine_lower");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:spine_mid");
		jointParentIndex = upperSpineJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:spine_upper");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:neck");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:head");
		jointParentIndex = upperSpineJointIndex;
		jointParentIndex = clavicleJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:clavicle");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulderblade_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulder_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:elbow_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:forearm_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:wrist_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hand_r");
		jointParentIndex = clavicleJointIndex;
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulderblade_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulder_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:elbow_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:forearm_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:wrist_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hand_l");
		jointParentIndex = rootJointIndex;
		jointParentIndex = pelvisJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:pelvis");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hip_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:knee_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shin_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:ankle_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:foot_r");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:toe_r");
		jointParentIndex = pelvisJointIndex;
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hip_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:knee_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shin_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:ankle_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:foot_l");
		jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:toe_l");

		// use index field as something more useful, like level in hierarchy from root
		for (jointIndex = 0; jointIndex < jointCount; ++jointIndex)
		{
			j = 0;
			jointParentIndex = hierarchy->nodes[jointIndex].parentIndex;
			while (jointParentIndex >= 0)
			{
				jointParentIndex = hierarchy->nodes[jointParentIndex].parentIndex;
				++j;
			}
			hierarchy->nodes[jointIndex].index = j;
		}

		// save hierarchy assets
		a3hierarchySaveBinary(hierarchy, fileStream);

		// done
		a3fileStreamClose(fileStream);
	}


	// next set up hierarchy poses
	demoState->demoMode_keyframes->editSkeletonIndex = 0;
	hierarchy = demoState->hierarchy_skel + demoState->demoMode_keyframes->editSkeletonIndex;
	hierarchyPoseGroup = demoState->hierarchyPoseGroup_skel + demoState->demoMode_keyframes->editSkeletonIndex;
	a3hierarchyPoseGroupCreate(hierarchyPoseGroup, hierarchy, 1);
	hierarchyPoseFlag = demoState->hierarchyPoseFlag_skel[demoState->demoMode_keyframes->editSkeletonIndex];

	// define "bind pose" or the initial transformation 
	//	description for each joint (not a literal transform)
	p = 0;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, 0.0f, 0.0, +3.6f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate | a3poseFlag_translate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_lower");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, -90.0f, -5.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, 0.0f, -0.1f, +0.1f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_mid");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, +10.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +2.0f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_upper");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, -5.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +2.0f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:neck");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +0.5f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:head");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +0.5f, 0.0f, 0.0f);
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:clavicle");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, +90.0f, 0.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -0.1f, +0.1f, 0.0f);
//	j = a3hierarchyGetNodeIndex(hierarchy, "skel:pelvis");
//	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
//	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, 0.0f, +0.1f, -0.1f);

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulderblade_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +0.5f, -0.1f, -0.5f);
	hierarchyPoseFlag[j] = a3poseFlag_translate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulder_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, +30.0f, -10.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +0.5f, 0.0f, +0.5f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:elbow_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, +20.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +2.0f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:forearm_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +1.0f, 0.0f, 0.0f);
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:wrist_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, -10.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +1.0f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:hand_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +0.5f, 0.0f, 0.0f);

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulderblade_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -0.5f, -0.1f, -0.5f);
	hierarchyPoseFlag[j] = a3poseFlag_translate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulder_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, -30.0f, +10.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -0.5f, 0.0f, +0.5f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:elbow_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, -20.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -2.0f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:forearm_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -1.0f, 0.0f, 0.0f);
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:wrist_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, +10.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -1.0f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:hand_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -0.5f, 0.0f, 0.0f);

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:hip_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, +90.0f, +10.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +1.0f, -0.1f, +0.5f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:knee_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, -20.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +2.0f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shin_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +1.0f, 0.0f, 0.0f);
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:ankle_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, +90.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +1.0f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:foot_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, +10.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +0.5f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:toe_r");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, +0.5f, 0.0f, 0.0f);

	j = a3hierarchyGetNodeIndex(hierarchy, "skel:hip_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, -90.0f, -10.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -1.0f, -0.1f, +0.5f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:knee_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, +20.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -2.0f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:shin_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -1.0f, 0.0f, 0.0f);
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:ankle_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, -90.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -1.0f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:foot_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetRotation(hierarchyNodePose, 0.0f, 0.0f, -10.0f, a3true);
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -0.5f, 0.0f, 0.0f);
	hierarchyPoseFlag[j] = a3poseFlag_rotate;
	j = a3hierarchyGetNodeIndex(hierarchy, "skel:toe_l");
	hierarchyNodePose = hierarchyPoseGroup->pose[p].nodePose + j;
	a3hierarchyNodePoseSetTranslation(hierarchyNodePose, -0.5f, 0.0f, 0.0f);


	// ****TO-DO: 
	//	-> set up additional poses



	// finally set up hierarchy states
	hierarchyPoseGroup = demoState->hierarchyPoseGroup_skel + demoState->demoMode_keyframes->editSkeletonIndex;
	hierarchyState = demoState->hierarchyState_skel + demoState->demoMode_keyframes->editSkeletonIndex;
	a3hierarchyStateCreate(hierarchyState, hierarchyPoseGroup);
}


//-----------------------------------------------------------------------------

// internal utility for refreshing drawable
inline void a3_refreshDrawable_internal(a3_VertexDrawable *drawable, a3_VertexArrayDescriptor *vertexArray, a3_IndexBuffer *indexBuffer)
{
	drawable->vertexArray = vertexArray;
	if (drawable->indexType)
		drawable->indexBuffer = indexBuffer;
}


// the handle release callbacks are no longer valid; since the library was 
//	reloaded, old function pointers are out of scope!
// could reload everything, but that would mean rebuilding GPU data...
//	...or just set new function pointers!
void a3demo_refresh(a3_DemoState* demoState)
{
	a3_BufferObject* currentBuff = demoState->drawDataBuffer,
		* const endBuff = currentBuff + demoStateMaxCount_drawDataBuffer;
	a3_VertexArrayDescriptor* currentVAO = demoState->vertexArray,
		* const endVAO = currentVAO + demoStateMaxCount_vertexArray;
	a3_DemoStateShaderProgram* currentProg = demoState->shaderProgram,
		* const endProg = currentProg + demoStateMaxCount_shaderProgram;
	a3_UniformBuffer* currentUBO = demoState->uniformBuffer,
		* const endUBO = currentUBO + demoStateMaxCount_uniformBuffer;
	a3_Texture* currentTex = demoState->texture,
		* const endTex = currentTex + demoStateMaxCount_texture;
	a3_Framebuffer* currentFBO = demoState->framebuffer,
		* const endFBO = currentFBO + demoStateMaxCount_framebuffer;

	// set pointers to appropriate release callback for different asset types
	while (currentBuff < endBuff)
		a3bufferHandleUpdateReleaseCallback(currentBuff++);
	while (currentVAO < endVAO)
		a3vertexArrayHandleUpdateReleaseCallback(currentVAO++);
	while (currentProg < endProg)
		a3shaderProgramHandleUpdateReleaseCallback((currentProg++)->program);
	while (currentUBO < endUBO)
		a3bufferHandleUpdateReleaseCallback(currentUBO++);
	while (currentTex < endTex)
		a3textureHandleUpdateReleaseCallback(currentTex++);
	while (currentFBO < endFBO)
		a3framebufferHandleUpdateReleaseCallback(currentFBO++);

	// re-link specific object pointers for different asset types
	currentBuff = demoState->vbo_staticSceneObjectDrawBuffer;

	currentVAO = demoState->vao_position;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_grid, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_pointlight, currentVAO, currentBuff);

	currentVAO = demoState->vao_position_color;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_axes, currentVAO, currentBuff);

	currentVAO = demoState->vao_position_texcoord;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_skybox, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_unitquad, currentVAO, currentBuff);

//	currentVAO = demoState->vao_position_texcoord_normal;
	currentVAO = demoState->vao_tangentbasis;
	currentVAO->vertexBuffer = currentBuff;
	a3_refreshDrawable_internal(demoState->draw_plane, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_sphere, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_cylinder, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_torus, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_teapot, currentVAO, currentBuff);
	a3_refreshDrawable_internal(demoState->draw_triangle, currentVAO, currentBuff);

	a3demo_initDummyDrawable_internal(demoState);

	demoState->hierarchyState_skel->poseGroup = demoState->hierarchyPoseGroup_skel;
	demoState->hierarchyPoseGroup_skel->hierarchy = demoState->hierarchy_skel;
}


//-----------------------------------------------------------------------------
