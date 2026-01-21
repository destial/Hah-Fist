#pragma once
#ifndef __MESHRENDERER_H
#define __MESHRENDERER_H
#include "AEEngine.h"

/*
* @brief Static namespace functions that handle creation and deletion of meshes, for memory optimizations
*/
namespace MeshRenderer {
	static AEGfxMeshDrawMode RenderMode = AE_GFX_MDM_TRIANGLES;

	AEGfxVertexList* GetCircleMesh(int slices = 50);

	AEGfxVertexList* GetCenterRectMesh();

	void Free();
}

#endif