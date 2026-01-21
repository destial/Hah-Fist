#include "MeshRenderer.hpp"
#include <map>

namespace MeshRenderer {

	static AEGfxVertexList* cornerRectMesh = nullptr;
	static std::map<int, AEGfxVertexList*> circleMeshes;

	AEGfxVertexList* GetCircle(int slices) {
		if (circleMeshes[slices]) {
			return circleMeshes[slices];
		}
		AEGfxMeshStart();
		for (int i = 0; i < slices; ++i) {
			f32 x = AESin(2.f * PI * i / slices);
			f32 y = AECos(2.f * PI * i / slices);

			f32 x2 = AESin(2.f * PI * (i + 1) / slices);
			f32 y2 = AECos(2.f * PI * (i + 1) / slices);

			// Add main triangle
			AEGfxTriAdd(
				0.5f, 0.5f, 0xFFFFFFFF, 0.5f, 0.5f,
				x + 1.f, y + 1.f, 0xFFFFFFFF, (x + 1.f) * 0.5f, 1.f - (y + 1.f) * 0.5f,
				x2 + 1.f, y2 + 1.f, 0xFFFFFFFF, (x2 + 1.f) * 0.5f, 1.f - (y2 + 1.f) * 0.5f
			);
		}
		return circleMeshes[slices] = AEGfxMeshEnd();
	}

	AEGfxVertexList* GetLeftBottomCornerRect() {
		if (cornerRectMesh) {
			return cornerRectMesh;
		}
		AEGfxMeshStart();
		AEGfxTriAdd(
			0.f, 0.f, 0xFFFFFFFF, 0.0f, 1.0f,
			1.f, 0.f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.f, 1.f, 0xFFFFFFFF, 0.0f, 0.0f);

		AEGfxTriAdd(
			1.f, 0.f, 0xFFFFFFFF, 1.0f, 1.0f,
			1.f, 1.f, 0xFFFFFFFF, 1.0f, 0.0f,
			0.f, 1.f, 0xFFFFFFFF, 0.0f, 0.0f);
		return cornerRectMesh = AEGfxMeshEnd();
	}

	void Free() {
		if (cornerRectMesh) {
			AEGfxMeshFree(cornerRectMesh);
			cornerRectMesh = nullptr;
		}
		for (auto& entry : circleMeshes) {
			AEGfxMeshFree(entry.second);
		}
		circleMeshes.clear();
	}
}
