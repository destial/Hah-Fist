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
	text_size{ 1.f },
	text_alignment{ TEXT_ALIGNMENT::CENTER },
	text_color{255, 0, 0, 0},
	overlay_text_color{255, 0, 0, 0},
	overlay_texture{ nullptr },
	overlay_color{ 255, 128, 128, 128 }
{
	layer = RenderLayer::UI;
	mesh = MeshRenderer::GetCenterRectMesh();
	font = AssetManager::GetFontId("Assets/buggy-font.ttf");
}

BaseUI::~BaseUI() {
	std::printf("Called BaseUI deconstructor\n");
}

void BaseUI::PostUpdate(const f32& dt) {
	BaseEntity::PostUpdate(dt);

	if (layer == RenderLayer::UI) {
		AEVec2 cam_pos{ 0.f };
		AEGfxGetCamPosition(&cam_pos.x, &cam_pos.y);
		cam_pos = Utils::ScreenToScale(cam_pos.x, cam_pos.y);
		transform = Utils::GetTransformMatrix(position + cam_pos, scale, rotation);
	}
	else {
		transform = Utils::GetTransformMatrix(position, scale, rotation);
	}
}

bool BaseUI::IsInteractive() const {
	return interactive;
}

void BaseUI::SetInteractive(bool i) {
	interactive = i;
}

void BaseUI::Render() {
	if (SceneManager::GetInstance()->GetEditor()->IsToggled()) {
		BaseUI::PostUpdate(Utils::GetDeltaTime());
	}
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
	RenderText();
}

void BaseUI::RenderText() {
	f32 w, h;
	const char* str = text.c_str();
	AEGfxGetPrintSize(font, str, text_size, &w, &h);
	f32 t_color[4] = {};
	t_color[0] = (interactive && mouse_hovered ? overlay_text_color : text_color).r / 255.f;
	t_color[1] = (interactive && mouse_hovered ? overlay_text_color : text_color).g / 255.f;
	t_color[2] = (interactive && mouse_hovered ? overlay_text_color : text_color).b / 255.f;
	t_color[3] = (interactive && mouse_hovered ? overlay_text_color : text_color).a / 255.f;
	if (text_alignment == TEXT_ALIGNMENT::CENTER) {
		AEVec2 screen = Utils::GameToTextScreen(this->position.x, this->position.y, layer != RenderLayer::UI);
		AEGfxPrint(font, str, screen.x - ((w / 2.f) / text_size * 0.5f), screen.y - ((h / 2.f) / text_size * 0.5f), h, t_color[0], t_color[1], t_color[2], t_color[3]);
	}
	else if (text_alignment == TEXT_ALIGNMENT::LEFT_CORNER) {
		AEVec2 screen = Utils::GameToTextScreen(this->position.x - (this->scale.x * 0.5f), this->position.y, layer != RenderLayer::UI);
		AEGfxPrint(font, str, screen.x, screen.y - ((h / 2.f) / text_size * 0.5f), h, t_color[0], t_color[1], t_color[2], t_color[3]);
	}
	else if (text_alignment == TEXT_ALIGNMENT::RIGHT_CORNER) {
		AEVec2 screen = Utils::GameToTextScreen(this->position.x + (this->scale.x * 0.5f), this->position.y, layer != RenderLayer::UI);
		AEGfxPrint(font, str, screen.x - ((w / 2.f) / text_size), screen.y - ((h / 2.f) / text_size * 0.5f), h, t_color[0], t_color[1], t_color[2], t_color[3]);
	}
}
