/*!
* @file ButtonUI.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 13 January 2026
* @course CSD1451
* @brief Definition file for a button UI element
*/

#include "ButtonUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Managers/SceneManager.hpp"

ButtonUI::ButtonUI(AEVec2 pos) : BaseUI(pos),
	click_listeners(0),
	hover_listeners(0),
	unhover_listeners(0)
{ // Ctor
	text = "ButtonUI";
}

ButtonUI::~ButtonUI() { // Dtor
	click_listeners.clear();
	unhover_listeners.clear();
	hover_listeners.clear();
}

/*!
* @brief Inherited: Update the UI
*/
void ButtonUI::Update(const f32& dt) {
	if (!active)
		return;

	BaseUI::Update(dt);
	if (!interactive)
		return;

	// Get mouse position
	s32 mouse_x, mouse_y;
	AEInputGetCursorPosition(&mouse_x, &mouse_y);
	AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };
	AEVec2 mouse_world = Utils::ScreenToWorld(mouse.x, mouse.y);

	// If mouse is hovered on this UI
	if (IsOn(mouse_world)) {
		OnMouseHover(mouse);
		mouse_hovered = true;

		// If mouse was clicked on this UI
		clicked_this_frame = AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_RBUTTON) || AEInputCheckTriggered(AEVK_MBUTTON);
		if (clicked_this_frame) {
			OnMouseClick(mouse,
				AEInputCheckTriggered(AEVK_LBUTTON) ? MouseButton::LEFT :
				AEInputCheckTriggered(AEVK_RBUTTON) ? MouseButton::RIGHT :
				MouseButton::MIDDLE
			);
		}
	}
	else if (mouse_hovered) { // Mouse no longer hovered this frame
		OnMouseStopHover();
		mouse_hovered = false;
		clicked_this_frame = false;
	}
}

void ButtonUI::Render() {
	if (!active)
		return;

	// If level editor is toggled, PostUpdate was not called so we force call it here
	if (SceneManager::GetInstance()->GetEditor()->IsToggled()) {
		BaseUI::PostUpdate(Utils::GetDeltaTime());
	}

	// Render image/mesh based on current interaction state
	if (image && image->data) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(image->data, 0.f, 0.f);
		if (mouse_hovered && overlay_texture && overlay_texture->data) {
			AEGfxTextureSet(overlay_texture->data, 0.f, 0.f);
		}
	}
	else {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		if (mouse_hovered && overlay_texture && overlay_texture->data) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(overlay_texture->data, 0.f, 0.f);
		}
	}
	AEGfxSetColorToMultiply(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
	if (mouse_hovered) {
		AEGfxSetColorToMultiply(overlay_color.r / 255.f, overlay_color.g / 255.f, overlay_color.b / 255.f, overlay_color.a / 255.f);
	}
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(color.a / 255.f);
	AEGfxSetTransform(this->transform.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);

	// Render overlay text
	RenderText();
}

/*!
* @brief Add a listener to this button when clicked by a mouse button
* @param func - The function listener object
*/
void ButtonUI::AddClickListener(std::function<void(MouseButton)> func) {
	click_listeners.push_back(func);
}

/*!
* @brief Add a listener to this button when hovered over
* @param func - The function listener object
*/
void ButtonUI::AddHoverListener(std::function<void()> func) {
	hover_listeners.push_back(func);
}

/*!
* @brief Add a listener to this button when unhovered
* @param func - The function listener object
*/
void ButtonUI::AddStopHoverListener(std::function<void()> func) {
	unhover_listeners.push_back(func);
}

/*!
* @brief Function to call when this button was clicked on
* @param mousePos - The mouse position
* @param button - The button clicked
*/
void ButtonUI::OnMouseClick(AEVec2, MouseButton button) {
	for (auto& func : click_listeners) {
		func(button);
	}
}

/*!
* @brief Function to call when this button is hovered over
* @param pos - The mouse position
*/
void ButtonUI::OnMouseHover(AEVec2) {
	for (auto& func : hover_listeners) {
		func();
	}
}

/*!
* @brief Function to call when this button was unhovered over
*/
void ButtonUI::OnMouseStopHover() {
	for (auto& func : unhover_listeners) {
		func();
	}
}

/*!
* @brief Function to check if mouse is on this button
* @brief Differs between Circle and Rectangle
* @param mouse - The mouse world position
* @return If the mouse is on this button
*/
bool ButtonUI::IsOn(AEVec2 const& mouse) {
	return Utils::OBBPoint(this, mouse);
}
