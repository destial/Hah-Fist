#include "ButtonUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"

ButtonUI::ButtonUI(AEVec2 pos) : BaseUI(pos),
	click_listeners(0),
	hover_listeners(0),
	unhover_listeners(0),
	clicked_this_frame(false),
	mouse_hovered(false)
{
	text = "Button";
}

ButtonUI::~ButtonUI() {
	std::printf("Called ButtonUI deconstructor\n");
	click_listeners.clear();
}

void ButtonUI::Update(const f32& dt) {
	BaseUI::Update(dt);
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

void ButtonUI::Render() {
	if (image && image->image) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		if (this->mouse_hovered && this->overlay_texture && this->overlay_texture->image) {
			AEGfxTextureSet(overlay_texture->image, 0.f, 0.f);
		}
		else {
			AEGfxTextureSet(image->image, 0.f, 0.f);
		}
	}
	else {
		if (this->mouse_hovered && this->overlay_texture && this->overlay_texture->image) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(overlay_texture->image, 0.f, 0.f);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		}
	}
	if (this->mouse_hovered) {
		AEGfxSetColorToMultiply(overlay_color.r / 255.f, overlay_color.g / 255.f, overlay_color.b / 255.f, overlay_color.a / 255.f);
	}
	else {
		AEGfxSetColorToMultiply(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
	}
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTransform(this->transform.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);
	RenderText();
}

void ButtonUI::AddClickListener(std::function<void(MouseButton)> func) {
	click_listeners.push_back(func);
}

void ButtonUI::AddHoverListener(std::function<void()> func) {
	hover_listeners.push_back(func);
}

void ButtonUI::AddStopHoverListener(std::function<void()> func) {
	unhover_listeners.push_back(func);
}

void ButtonUI::OnMouseClick(AEVec2 mousePos, MouseButton button) {
	for (auto& func : click_listeners) {
		func(button);
	}
}

void ButtonUI::OnMouseHover(AEVec2 pos) {
	for (auto& func : hover_listeners) {
		func();
	}
}

void ButtonUI::OnMouseStopHover() {
	for (auto& func : unhover_listeners) {
		func();
	}
}
