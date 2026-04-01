/*!
* @file BarUI.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 9 March 2026
* @course CSD1451
* @brief Definition file for a slider bar UI element
*/

#include "BarUI.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Managers/SceneManager.hpp"

BarUI::BarUI(AEVec2 pos) : BaseUI{ pos }, mouse_dragged{ false }, value { 0.f }, overlay_transform{ 1.f } {
	// Empty ctor body
}

BarUI::~BarUI() {} // Empty dtor

/*!
* @brief Inherited: Update the UI after input has been processed
*/
void BarUI::Update(const f32& dt) {
	if (!active)
		return;

	BaseUI::Update(dt);
	// Only process if interactive
	if (!interactive)
		return;
	
	// Get mouse position
	s32 mouse_x, mouse_y;
	AEInputGetCursorPosition(&mouse_x, &mouse_y);
	AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };
	AEVec2 mouse_world = Utils::ScreenToWorld(mouse.x, mouse.y);

	// If mouse is hovered on this UI
	if (Utils::OBBPoint(this, mouse_world)) {
		mouse_hovered = true;

		// If mouse was clicked on this UI
		clicked_this_frame = AEInputCheckTriggered(AEVK_LBUTTON);

		// If mouse started dragging on this UI
		if (!mouse_dragged) {
			mouse_dragged = clicked_this_frame;
		}
	}
	else if (mouse_hovered) { // Mouse no longer hovered this frame
		mouse_hovered = false;
		clicked_this_frame = false;
	}

	if (mouse_dragged) {
		// If mouse was dragged on this UI, update slider
		if (AEInputCheckCurr(AEVK_LBUTTON)) {
			AEVec2 local_mouse = mouse_world - (position - (scale * 0.5f));
			AEVec2Rotate(&local_mouse, &local_mouse, -rotation);
			value = local_mouse.x / scale.x;
		}
		else {
			mouse_dragged = false;
		}
	}
}

/*!
* @brief Inherited: Post-update the UI after everything else has been processed
*/
void BarUI::PostUpdate(const f32& dt) {
	if (!active)
		return;

	BaseUI::PostUpdate(dt);

	// Initialize position & scale of overlay value
	AEVec2 pos{ this->position.x, this->position.y }, scl{ this->scale.x, this->scale.y };

	// Clamp value between 0-1
	value = AEClamp(value, 0.f, 1.f);

	// Calculate position & scale of overlay value
	f32 w = scl.x;
	scl.x *= value;
	AEVec2 dir{ -1.f, 0.f };
	AEVec2Rotate(&dir, &dir, this->rotation);
	dir.x *= w * (1.f - value) * 0.5f;
	dir.y *= w * (1.f - value) * 0.5f;
	pos += dir;

	// Update overlay transform
	if (layer == RenderLayer::UI) { // On screen, so negate camera position
		AEVec2 cam_pos{ 0.f };
		AEGfxGetCamPosition(&cam_pos.x, &cam_pos.y);
		cam_pos = Utils::ScreenToScale(cam_pos.x, cam_pos.y);
		overlay_transform = Utils::GetTransformMatrix(pos + cam_pos, scl, this->rotation);
	}
	else { // In world, so use camera position
		overlay_transform = Utils::GetTransformMatrix(pos, scl, this->rotation);
	}
}

/*!
* @brief Inherited: Render the UI to the screen
*/
void BarUI::Render() {
	if (!active)
		return;

	// If level editor is toggled, PostUpdate was not called so we force call it here
	if (SceneManager::GetInstance()->GetEditor()->IsToggled()) {
		PostUpdate(Utils::GetDeltaTime());
	}

	// Render background image/mesh
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

	// Render overlay image/mesh
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

	// Render overlay text
	RenderText();
}

/*!
* @brief Get the value of the slider
* @return A value between 0-1
*/
f32 BarUI::GetValue() const {
	return value;
}

/*!
* @brief Set the value of the slider
* @param v - A value between 0-1
*/
void BarUI::SetValue(f32 v) {
	value = v;
}

/*!
* @brief Get the state of dragging of this slider
* @return The mouse drag state
*/
bool BarUI::IsDragging() const {
	return mouse_dragged;
}
