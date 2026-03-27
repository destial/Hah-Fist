/*!
* @file MeshRenderer.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 12 January 2026
* @course CSD1451
* @brief Declaration file for creating meshes which are used globally, to save memory
*/

#pragma once
#ifndef __MESHRENDERER_H
#define __MESHRENDERER_H
#include "AEEngine.h"

/*
* @brief Static namespace functions that handle creation and deletion of meshes, for memory optimizations
*/
namespace MeshRenderer {
	static AEGfxMeshDrawMode RenderMode = AE_GFX_MDM_TRIANGLES;

	/*!
	* @brief Create a circle mesh
	* @param slices - The number of slices the circle has
	* @return The AE mesh created
	*/
	AEGfxVertexList* GetCircleMesh(int slices = 50);

	/*!
	* @brief Create a rectangle mesh centered at the center
	* @return The AE mesh created
	*/
	AEGfxVertexList* GetCenterRectMesh();

	/*!
	* @brief Free all the created AE mesh
	*/
	void Free();
}

#endif