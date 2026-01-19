#pragma once
#ifndef __MESHRENDERER_H
#define __MESHRENDERER_H
#include "AEEngine.h"

namespace MeshRenderer {
	static AEGfxMeshDrawMode RenderMode = AE_GFX_MDM_TRIANGLES;

	AEGfxVertexList* CreateCircle(u32 color = 0xFFFFFFFF, int slices = 50);

	AEGfxVertexList* CreateCenterRect(u32 color = 0xFFFFFFFF);

	AEGfxVertexList* CreateLeftBottomCornerRect(u32 color = 0xFFFFFFFF);
}

#endif