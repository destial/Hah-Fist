#include "BarUI.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Managers/SceneManager.hpp"

BarUI::BarUI(AEVec2 pos) 
	: BaseUI{ pos }, value{ 0.f }, overlay_transform{ 1.f } {
}

BarUI::~BarUI() {}

void BarUI::Update(const f32& dt) {
	BaseUI::Update(dt);
	if (!interactive)
		return;

	s32 mouse_x, mouse_y;
	AEInputGetCursorPosition(&mouse_x, &mouse_y);
	AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };
	AEVec2 mouse_world = Utils::ScreenToWorld(mouse.x, mouse.y);

	if (Utils::OBBPoint(this, mouse_world)) {
		this->mouse_hovered = true;

		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			this->clicked_this_frame = true;
		}
		else {
			this->clicked_this_frame = false;
		}

		if (AEInputCheckCurr(AEVK_LBUTTON)) {
			AEVec2 local_mouse = mouse_world - (this->position - (this->scale * 0.5f));
			AEVec2Rotate(&local_mouse, &local_mouse, -this->rotation);
			value = local_mouse.x / this->scale.x;
		}
	}
	else if (this->mouse_hovered) {
		this->mouse_hovered = false;
		this->clicked_this_frame = false;
	}
}

void BarUI::PostUpdate(const f32& dt) {
	BaseUI::PostUpdate(dt);
	AEVec2 pos{ this->position.x, this->position.y }, scl{ this->scale.x, this->scale.y };
	value = AEClamp(value, 0.f, 1.f);
	f32 w = scl.x;
	scl.x *= value;
	AEVec2 dir{ -1.f, 0.f };
	AEVec2Rotate(&dir, &dir, this->rotation);
	dir.x *= w * (1.f - value) * 0.5f;
	dir.y *= w * (1.f - value) * 0.5f;
	pos += dir;

	if (layer == RenderLayer::UI) {
		AEVec2 cam_pos{ 0.f };
		AEGfxGetCamPosition(&cam_pos.x, &cam_pos.y);
		cam_pos = Utils::ScreenToScale(cam_pos.x, cam_pos.y);
		overlay_transform = Utils::GetTransformMatrix(pos + cam_pos, scl, this->rotation);
	}
	else {
		overlay_transform = Utils::GetTransformMatrix(pos, scl, this->rotation);
	}
}

void BarUI::Render() {
	if (SceneManager::GetInstance()->GetEditor()->IsToggled()) {
		PostUpdate(Utils::GetDeltaTime());
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

	if (overlay_texture && overlay_texture->data) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(overlay_texture->data, 0.f, 0.f);
	}
	else {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}
	AEGfxSetColorToMultiply(overlay_color.r / 255.f, overlay_color.g / 255.f, overlay_color.b / 255.f, overlay_color.a / 255.f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetTransparency(overlay_color.a / 255.f);
	AEGfxSetTransform(overlay_transform.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);

	RenderText();
}

f32 BarUI::GetValue() const {
	return value;
}

void BarUI::SetValue(f32 v) {
	value = v;
}
