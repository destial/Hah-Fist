/*!
* @file Debug.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 11 March 2026
* @course CSD1451
* @brief Declaration file for debugging unto the screen
*/

#include "Debug.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Managers/AssetManager.hpp"
#include <vector>

namespace DebugUtils {
	struct DebugRender {
		AEGfxVertexList* mesh{nullptr};
		AEMtx33 transform{ 0 };

		Color color;
	};

	static AEGfxVertexList* point = nullptr;
	static AEGfxVertexList* line = nullptr;
	bool enabled = false;
	std::vector<DebugRender> renders{ 0 };

	/*!
	* @brief If the debug tool is currently rendering to the screen
	* @return The render state
	*/
	bool IsRendering() {
		return enabled;
	}

	/*!
	* @brief Toggle the debug tool render state
	* @param state - The render state
	*/
	void ToggleRender(bool state) {
		enabled = state;
	}

	/*!
	* @brief Render a debug text unto the screen
	* @param world_pos - The world coordinates
	* @param text - The text to display
	* @param color - The text color
	* @param size - The text size
	* @param cam - If to take account camera position
	*/
	void RenderText(AEVec2 world_pos, std::string text, Color color, f32 size, bool cam) {
		u8 font = AssetManager::GetFontId(ASSET_DEBUG_FONT);
		AEVec2 screen = Utils::GameToTextScreen(world_pos.x, world_pos.y, cam);
		const char* str = text.c_str();
		f32 w, h;
		AEGfxGetPrintSize(font, str, size * Utils::ScreenSizeMultiplier().x, &w, &h);
		AEGfxPrint(font, str, screen.x, screen.y - ((h / 2.f) / size * 0.5f), h, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
	}

	/*!
	* @brief Render a debug text unto the screen
	* @param world_pos - The world coordinates
	* @param text - The text to display
	* @param cam - If to take account camera position
	*/
	void RenderText(AEVec2 world_pos, std::string text, bool cam) {
		RenderText(world_pos, text, { 255, 0, 255, 0 }, 2.5f, cam);
	}

	/*!
	* @brief Render a debug point unto the screen
	* @param world_pos - The world coordinates
	* @param color - The point color
	*/
	void RenderPoint(AEVec2 world_pos, Color color) {
		if (point == nullptr) {
			point = MeshRenderer::GetCircleMesh(4);
		}
		AEMtx33 scale = { 1.f };
		AEMtx33Identity(&scale);
		AEMtx33Scale(&scale, 3.f, 3.f);
		AEVec2 screenPos = Utils::GameToScreen(world_pos.x, world_pos.y);
		AEMtx33 translate = { 0 };
		AEMtx33Identity(&translate);
		AEMtx33Trans(&translate, screenPos.x, screenPos.y);

		renders.push_back({ point, translate * scale, color });
	}

	/*!
	* @brief Render a debug line unto the screen
	* @param point_a - The start of the line
	* @param point_b - The end of the line
	* @param color - The line color
	*/
	void RenderLine(AEVec2 point_a, AEVec2 point_b, Color color) {
		if (line == nullptr) {
			line = MeshRenderer::GetCenterRectMesh();
		}
		AEVec2 midpoint = (point_a + point_b) * 0.5f;
		AEVec2 dir = point_b - point_a;
		AEVec2 up = { 0.f, 1.f };
		f32 rotation = AEVec2AngleCCW(&up, &dir);
		AEMtx33 scale = { 1.f };
		AEVec2 s_scale = Utils::ScaleToScreen(1.f, AEVec2Length(&dir));
		AEMtx33Identity(&scale);
		AEMtx33Scale(&scale, 3.f, s_scale.y); // Default thickness to 3 pixels
		AEMtx33 rotate = { 0 };
		AEMtx33Identity(&rotate);
		AEMtx33Rot(&rotate, rotation);
		AEVec2 screenPos = Utils::GameToScreen(midpoint.x, midpoint.y);
		AEMtx33 translate = { 0 };
		AEMtx33Identity(&translate);
		AEMtx33Trans(&translate, screenPos.x, screenPos.y);

		renders.push_back({ line, translate * rotate * scale, color });
	}

	/*!
	* @brief Private function to render all the lines and points if rendering
	*/
	void _RenderAll() {
		if (enabled) {
			for (DebugRender& r : renders) {
				AEGfxTextureSet(nullptr, 0.f, 0.f);
				AEGfxSetRenderMode(AE_GFX_RM_COLOR);
				AEGfxSetColorToMultiply(r.color.r / 255.f, r.color.g / 255.f, r.color.b / 255.f, r.color.a / 255.f);
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetTransparency(1.0f);
				AEGfxSetTransform(r.transform.m);
				AEGfxMeshDraw(r.mesh, MeshRenderer::RenderMode);
			}
		}
		renders.clear();
	}
}