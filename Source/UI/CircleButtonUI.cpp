#include "CircleButtonUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"

CircleButtonUI::CircleButtonUI(AEVec2 pos) : BaseUI(pos),
	click_listeners(0),
	hover_listeners(0),
	unhover_listeners(0),
	radius(1.f)
{
	text = "CircleButtonUI";
	mesh = MeshRenderer::GetCircleMesh();
}

CircleButtonUI::~CircleButtonUI() {
	std::printf("Called CircleButtonUI deconstructor\n");
	click_listeners.clear();
	unhover_listeners.clear();
	hover_listeners.clear();
}

void CircleButtonUI::Update(const f32& dt) {
	BaseUI::Update(dt);
	s32 mouse_x, mouse_y;
	AEInputGetCursorPosition(&mouse_x, &mouse_y);
	AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };
	AEVec2 mouse_world = Utils::Screen_To_World(mouse.x, mouse.y);

	if (AEVec2Distance(&mouse_world, &this->position) <= this->radius) {
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

void CircleButtonUI::PostUpdate(const f32& dt) {
	this->scale = { this->radius, this->radius };
	BaseEntity::PostUpdate(dt);
}

void CircleButtonUI::Render() {
	if (image && image->image) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(image->image, 0.f, 0.f);
	}
	else {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}
	AEGfxSetColorToMultiply(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
	if (this->mouse_hovered) {
		if (this->overlay_texture && this->overlay_texture->image) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(overlay_texture->image, 0.f, 0.f);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		}
		AEGfxSetColorToMultiply(overlay_color.r / 255.f, overlay_color.g / 255.f, overlay_color.b / 255.f, overlay_color.a / 255.f);
	}
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTransform(this->transform.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);
	RenderText();
}

void CircleButtonUI::AddClickListener(std::function<void(MouseButton)> func) {
	click_listeners.push_back(func);
}

void CircleButtonUI::AddHoverListener(std::function<void()> func) {
	hover_listeners.push_back(func);
}

void CircleButtonUI::AddStopHoverListener(std::function<void()> func) {
	unhover_listeners.push_back(func);
}

void CircleButtonUI::OnMouseClick(AEVec2 mousePos, MouseButton button) {
	for (auto& func : click_listeners) {
		func(button);
	}
}

void CircleButtonUI::OnMouseHover(AEVec2 pos) {
	for (auto& func : hover_listeners) {
		func();
	}
}

void CircleButtonUI::OnMouseStopHover() {
	for (auto& func : unhover_listeners) {
		func();
	}
}
