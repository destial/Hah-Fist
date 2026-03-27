/*!
* @file CircleButtonUI.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 13 January 2026
* @course CSD1451
* @brief Definition file for a circular button UI element
*/

#include "CircleButtonUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Managers/SceneManager.hpp"

CircleButtonUI::CircleButtonUI(AEVec2 pos) : ButtonUI{ pos }, radius{ 1.f } { // Ctor
	text = "CircleButtonUI";
	mesh = MeshRenderer::GetCircleMesh();
}

CircleButtonUI::~CircleButtonUI() {} // Empty dtor

/*!
* @brief Inherited: Post-update the UI after everything has been processed
* @brief Update's the scale based on radius
*/
void CircleButtonUI::PostUpdate(const f32& dt) {
	scale = { radius, radius };
	ButtonUI::PostUpdate(dt);
}

/*!
* @brief Inherited: Function to check if mouse is on this button
* @brief Differs between Circle and Rectangle
* @param mouse - The mouse world position
* @return If the mouse is on this button
*/
bool CircleButtonUI::IsOn(AEVec2 const& mouse) {
	AEVec2 m{ mouse };
	return AEVec2Distance(&m, &position) <= radius;
}
