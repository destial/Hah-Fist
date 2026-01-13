#include "BaseUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include <cstdio>

BaseUI::BaseUI(AEVec2 pos) : BaseEntity(pos),
	overlay_texture(nullptr),
	overlay_color(0xFFDDDDDD),
	mouse_hovered(false),
	clicked_this_frame(false)
{
	mesh = MeshRenderer::CreateLeftBottomCornerRect(0xFFFFFFFF);
}

BaseUI::BaseUI() : BaseEntity(), 
	overlay_texture(nullptr),
	overlay_color(0xFFDDDDDD),
	mouse_hovered(false),
	clicked_this_frame(false) {
	mesh = MeshRenderer::CreateLeftBottomCornerRect(0xFFFFFFFF);
}

BaseUI::~BaseUI() {
	std::printf("Called BaseUI deconstructor\n");
	if (overlay_texture) {
		AEGfxTextureUnload(overlay_texture);
		overlay_texture = nullptr;
	}
}

void BaseUI::Update(const f32& dt) {
	s32 mouse_x, mouse_y;
	AEInputGetCursorPosition(&mouse_x, &mouse_y);
	AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };
	AEVec2 mouse_world = Screen_To_World(mouse.x, mouse.y);

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
	BaseEntity::Render();

	if (this->mouse_hovered) {
		if (this->overlay_texture) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(overlay_texture, 0.f, 0.f);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		}
		u32 color = overlay_color;
		float alpha = ((color >> 24) & 255) / 255.f;
		float red = ((color >> 16) & 255) / 255.f;
		float green = ((color >> 8) & 255) / 255.f;
		float blue = (color & 255) / 255.f;
		AEGfxSetColorToMultiply(red, green, blue, alpha);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);
		AEGfxSetTransform(this->transform.m);
		AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
		AEGfxTextureSet(nullptr, 0.f, 0.f);
	}
}