#pragma once
#ifndef __MESHRENDERER_H
#define __MESHRENDERER_H
#include "AEEngine.h"

namespace MeshRenderer {
	static AEGfxMeshDrawMode RenderMode = AE_GFX_MDM_TRIANGLES;

	AEGfxVertexList* CreateCircle(int slices, u32 color);

	AEGfxVertexList* CreateCenterRect(u32 color);

	AEGfxVertexList* CreateLeftBottomCornerRect(u32 color);
}

#endif