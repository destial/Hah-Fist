#pragma once
#ifndef __MESHRENDERER_H
#define __MESHRENDERER_H
#include "AEEngine.h"

namespace MeshRenderer {
	static AEGfxMeshDrawMode RenderMode = AE_GFX_MDM_TRIANGLES;

	static AEGfxVertexList* CreateCircle(int slices, u32 color) {
		AEGfxMeshStart();
		for (int i = 0; i < slices; ++i) {
			f32 x = AESin(2.f * PI * i / slices);
			f32 y = AECos(2.f * PI * i / slices);

			f32 x2 = AESin(2.f * PI * (i + 1) / slices);
			f32 y2 = AECos(2.f * PI * (i + 1) / slices);

			// Add main triangle
			AEGfxTriAdd(
				0.f, 0.f, color, 0.5f, 0.5f,
				x, y, color, (x + 1.f) * 0.5f, (y + 1.f) * 0.5f,
				x2, y2, color, (x2 + 1.f) * 0.5f, (y2 + 1.f) * 0.5f
			);
		}
		return AEGfxMeshEnd();
	}

	static AEGfxVertexList* CreateCenterRect(u32 color) {
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, color, 0.0f, 1.0f,
			0.5f, -0.5f, color, 1.0f, 1.0f,
			-0.5f, 0.5f, color, 0.0f, 0.0f);

		AEGfxTriAdd(
			0.5f, -0.5f, color, 1.0f, 1.0f,
			0.5f, 0.5f, color, 1.0f, 0.0f,
			-0.5f, 0.5f, color, 0.0f, 0.0f);
		return AEGfxMeshEnd();
	}

	static AEGfxVertexList* CreateLeftBottomCornerRect(u32 color) {
		AEGfxMeshStart();
		AEGfxTriAdd(
			0.f, 0.f, color, 0.0f, 1.0f,
			1.f, 0.f, color, 1.0f, 1.0f,
			0.f, 1.f, color, 0.0f, 0.0f);

		AEGfxTriAdd(
			1.f, 0.f, color, 1.0f, 1.0f,
			1.f, 1.f, color, 1.0f, 0.0f,
			0.f, 1.f, color, 0.0f, 0.0f);
		return AEGfxMeshEnd();
	}
}

#endif