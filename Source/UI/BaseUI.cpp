#include "BaseUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Managers/AssetManager.hpp"
#include <iostream>

BaseUI::BaseUI(AEVec2 pos) : BaseEntity(pos),
	overlay_texture(nullptr),
	overlay_color(0xFFDDDDDD),
	base_color(0xFFFFFFFF),
	mouse_hovered(false),
	clicked_this_frame(false),
	font(0),
	text(""),
	text_size(1.f)
{
	mesh = MeshRenderer::CreateLeftBottomCornerRect(0xFFFFFFFF);
	font = AssetManager::GetFontId("Assets/buggy-font.ttf");
}

BaseUI::~BaseUI() {
	std::printf("Called BaseUI deconstructor\n");
}

void BaseUI::Update(const f32& dt) {
	BaseEntity::Update(dt);
	s32 mouse_x, mouse_y;
	AEInputGetCursorPosition(&mouse_x, &mouse_y);
	AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };
	AEVec2 mouse_world = Utils::Screen_To_World(mouse.x, mouse.y);

	if (mouse_world.x >= this->position.x && mouse_world.x <= this->position.x + this->scale.x &&
		mouse_world.y >= this->position.y && mouse_world.y <= this->position.y + this->scale.y) {
		OnMouseHover(mouse);
		this->mouse_hovered = true;

		if (AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_RBUTTON) || AEInputCheckTriggered(AEVK_MBUTTON)) {
			this->clicked_this_frame = true;
			OnMouseClick(mouse,
				AEInputCheckTriggered(AEVK_LBUTTON) ? MouseButton::LEFT :
				AEInputCheckTriggered(AEVK_RBUTTON) ? MouseButton::RIGHT :
				MouseButton::MIDDLE
			);
		}
		else {
			this->clicked_this_frame = false;
		}
	}
	else if (this->mouse_hovered) {
		OnMouseStopHover();
		this->mouse_hovered = false;
		this->clicked_this_frame = false;
	}
}

void BaseUI::Render() {
	if (texture) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(texture, 0.f, 0.f);
	}
	else {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}
	Color c = Utils::ConvertFromColor(base_color);
	AEGfxSetColorToMultiply(c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f);
	if (this->mouse_hovered) {
		if (this->overlay_texture) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(overlay_texture, 0.f, 0.f);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		}
		c = Utils::ConvertFromColor(overlay_color);
		AEGfxSetColorToMultiply(c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f);
	}
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTransform(this->transform.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);
	RenderText();
}

void BaseUI::RenderText() {
	AEVec2 world = Utils::Game_To_Screen(this->position.x, this->position.y);
	f32 w, h;
	const char* str = text.c_str();
	AEGfxGetPrintSize(font, str, text_size, &w, &h);
	AEGfxPrint(font, str, world.x, world.y, h, 0.f, 0.f, 0.f, 1.f);
}
