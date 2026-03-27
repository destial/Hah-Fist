/*!
* @file MeshRenderer.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 12 January 2026
* @course CSD1451
* @brief Declaration file for creating meshes which are used globally, to save memory
*/

#include "MeshRenderer.hpp"
#include <map>

namespace MeshRenderer {

	static AEGfxVertexList* centerRectMesh{ nullptr };
	static std::map<int, AEGfxVertexList*> circleMeshes;

	/*!
	* @brief Create a circle mesh
	* @param slices - The number of slices the circle has
	* @return The AE mesh created
	*/
	AEGfxVertexList* GetCircleMesh(int slices) {
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
				0.f, 0.f, 0xFFFFFFFF, 0.5f, 0.5f,
				x, y, 0xFFFFFFFF, (x + 1.f) * 0.5f, 1.f - (y + 1.f) * 0.5f,
				x2, y2, 0xFFFFFFFF, (x2 + 1.f) * 0.5f, 1.f - (y2 + 1.f) * 0.5f
			);
		}
		return circleMeshes[slices] = AEGfxMeshEnd();
	}

	/*!
	* @brief Create a rectangle mesh centered at the center
	* @return The AE mesh created
	*/
	AEGfxVertexList* GetCenterRectMesh() {
		if (centerRectMesh) {
			return centerRectMesh;
		}
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		return centerRectMesh = AEGfxMeshEnd();
	}

	/*!
	* @brief Free all the created AE mesh
	*/
	void Free() {
		if (centerRectMesh) {
			AEGfxMeshFree(centerRectMesh);
			centerRectMesh = nullptr;
		}
		for (auto& entry : circleMeshes) {
			AEGfxMeshFree(entry.second);
		}
		circleMeshes.clear();
	}
}
