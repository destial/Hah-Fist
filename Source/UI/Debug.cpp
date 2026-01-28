#include "Debug.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/AEOverload.hpp"

namespace DebugUtils {
	static AEGfxVertexList* point = nullptr;
	static AEGfxVertexList* line = nullptr;

	void RenderPoint(AEVec2 world_pos, Color color) {
		if (point == nullptr) {
			point = MeshRenderer::GetCircleMesh(4);
		}
		AEMtx33 scale = { 1.f };
		AEMtx33Identity(&scale);
		AEMtx33Scale(&scale, 3.f, 3.f);
		AEVec2 screenPos = Utils::Game_To_Screen(world_pos.x, world_pos.y);
		AEMtx33 translate = { 0 };
		AEMtx33Identity(&translate);
		AEMtx33Trans(&translate, screenPos.x, screenPos.y);
		AEMtx33 transform = translate * scale;

		AEGfxTextureSet(nullptr, 0.f, 0.f);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToMultiply(color.r / 255.f, color.b / 255.f, color.g / 255.f, color.a / 255.f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(point, MeshRenderer::RenderMode);
	}

	void RenderLine(AEVec2 point_a, AEVec2 point_b, Color color) {
		if (line == nullptr) {
			line = MeshRenderer::GetCenterRectMesh();
		}

		AEVec2 midpoint = (point_a + point_b) * 0.5f;
		AEVec2 dir = point_b - point_a;
		AEVec2 up = { 0.f, 1.f };
		f32 rotation = AEVec2AngleCCW(&up, &dir);

		AEMtx33 scale = { 1.f };
		AEVec2 s_scale = Utils::Scale_To_Screen(1.f, AEVec2Length(&dir));
		AEMtx33Identity(&scale);
		AEMtx33Scale(&scale, 3.f, s_scale.y);
		AEMtx33 rotate = { 0 };
		AEMtx33Identity(&rotate);
		AEMtx33Rot(&rotate, rotation);
		AEVec2 screenPos = Utils::Game_To_Screen(midpoint.x, midpoint.y);
		AEMtx33 translate = { 0 };
		AEMtx33Identity(&translate);
		AEMtx33Trans(&translate, screenPos.x, screenPos.y);
		AEMtx33 transform = translate * rotate * scale;

		AEGfxTextureSet(nullptr, 0.f, 0.f);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToMultiply(color.r / 255.f, color.b / 255.f, color.g / 255.f, color.a / 255.f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(line, MeshRenderer::RenderMode);
	}
}