/*!
* @file BaseUI.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 13 January 2026
* @course CSD1451
* @brief Definition file for a base UI element
*/

#include "BaseUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/SceneManager.hpp"
#include <iostream>

BaseUI::BaseUI(AEVec2 pos) : BaseEntity{ pos },
	interactive{true},
	font{0},
	clicked_this_frame{ false },
	mouse_hovered{ false },
	text{ "BaseUI" },
	text_size{ 7.5f },
	text_alignment{ TextAlignment::CENTER },
	text_color{255, 0, 0, 0},
	overlay_text_color{255, 0, 0, 0},
	overlay_texture{ nullptr },
	overlay_color{ 255, 128, 128, 128 }
{ // Ctor
	layer = RenderLayer::UI;
	mesh = MeshRenderer::GetCenterRectMesh();
	font = AssetManager::GetFontId(ASSET_DEFAULT_FONT);
}

BaseUI::~BaseUI() {} // Empty dtor

/*!
* @brief Post-update the UI after everything else has been processed
*/
void BaseUI::PostUpdate(const f32& dt) {
	BaseEntity::PostUpdate(dt);

	// On screen, so negate camera position
	if (layer == RenderLayer::UI || layer == RenderLayer::BACKGROUND) {
		AEVec2 cam_pos{ 0.f };
		AEGfxGetCamPosition(&cam_pos.x, &cam_pos.y);
		cam_pos = Utils::ScreenToScale(cam_pos.x, cam_pos.y);
		transform = Utils::GetTransformMatrix(position + cam_pos, scale, rotation);
	}
	else { // In world, so use camera position
		transform = Utils::GetTransformMatrix(position, scale, rotation);
	}
}

/*!
* @brief Render the UI to the screen
*/
void BaseUI::Render() {
	// If level editor is toggled, PostUpdate was not called so we force call it here
	if (SceneManager::GetInstance()->GetEditor()->IsToggled()) {
		BaseUI::PostUpdate(Utils::GetDeltaTime());
	}

	// Render image/mesh
	if (image && image->data) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(image->data, 0.f, 0.f);
	}
	else {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}
	AEGfxSetColorToMultiply(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(color.a / 255.f);
	AEGfxSetTransform(this->transform.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);

	// Render the overlay text
	RenderText();
}

/*!
* @brief Render the UI text to the screen
*/
void BaseUI::RenderText() {
	// Split the lines into its newline (if any)
	std::vector<std::string> lines;
	size_t start = 0;
	size_t nline = text.find('|');
	while (nline != text.npos) {
		lines.push_back(text.substr(start, text.find('|', start) - start));
		start = nline + 1;
		nline = text.find('|', start);
	}
	lines.push_back(text.substr(start));

	// Render each line as it's own text
	f32 w, h;
	AEGfxGetPrintSize(font, text.c_str(), text_size * Utils::ScreenSizeMultiplier().x, &w, &h);
	f32 start_h = static_cast<f32>((lines.size() - 1) * ((h / 2.f) / text_size * 0.5f));
	
	for (std::string const& line : lines) {
		const char* str = line.c_str();
		AEGfxGetPrintSize(font, str, text_size * Utils::ScreenSizeMultiplier().x, &w, &h);

		// If interactive & was hovered, use hovered color
		f32 t_color[4] = {};
		t_color[0] = (interactive && mouse_hovered ? overlay_text_color : text_color).r / 255.f;
		t_color[1] = (interactive && mouse_hovered ? overlay_text_color : text_color).g / 255.f;
		t_color[2] = (interactive && mouse_hovered ? overlay_text_color : text_color).b / 255.f;
		t_color[3] = (interactive && mouse_hovered ? overlay_text_color : text_color).a / 255.f;

		// Render text based on alignment variable
		// Calculate screen position based on render layer
		// UI or BACKGROUND == on screen, anything else is in world
		if (text_alignment == TextAlignment::CENTER) {
			AEVec2 screen = Utils::GameToTextScreen(this->position.x, this->position.y, (layer != RenderLayer::UI && layer != RenderLayer::BACKGROUND));
			AEGfxPrint(font, str, screen.x - ((w / 2.f) / text_size * 0.5f), start_h + screen.y - ((h / 2.f) / text_size * 0.5f), h, t_color[0], t_color[1], t_color[2], t_color[3]);
		}
		else if (text_alignment == TextAlignment::LEFT_CORNER) {
			AEVec2 screen = Utils::GameToTextScreen(this->position.x - (this->scale.x * 0.5f), this->position.y, (layer != RenderLayer::UI && layer != RenderLayer::BACKGROUND));
			AEGfxPrint(font, str, screen.x, screen.y - ((h / 2.f) / text_size * 0.5f),start_h + h, t_color[0], t_color[1], t_color[2], t_color[3]);
		}
		else if (text_alignment == TextAlignment::RIGHT_CORNER) {
			AEVec2 screen = Utils::GameToTextScreen(this->position.x + (this->scale.x * 0.5f), this->position.y, (layer != RenderLayer::UI && layer != RenderLayer::BACKGROUND));
			AEGfxPrint(font, str, screen.x - ((w / 2.f) / text_size), start_h + screen.y - ((h / 2.f) / text_size * 0.5f), h, t_color[0], t_color[1], t_color[2], t_color[3]);
		}

		// Next line, go down in height
		start_h -= ((h / 2.f) / text_size * (text_size * 0.2f));
	}
}

/*!
* @brief Get whether this element is interactive
* @return This element's interactivity
*/
bool BaseUI::IsInteractive() const {
	return interactive;
}

/*!
* @brief Set the element's interactivity
* @param i - The interactive state
*/
void BaseUI::SetInteractive(bool i) {
	interactive = i;
}