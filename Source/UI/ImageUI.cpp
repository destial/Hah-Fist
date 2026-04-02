/*!
* @file ImageUI.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 10 March 2026
* @course CSD1451
* @brief Definition file for an image UI element
*/

#include "ImageUI.hpp"
#include "../Managers/AssetManager.hpp"

ImageUI::ImageUI(const char* file_name, AEVec2 pos, int r, int c)
: BaseUI{ pos },
  animationFrame{0}, animationTimer{0}, currentCol{0}, currentRow{0},
  sprite{ r == 0 ? nullptr : AssetManager::GetSpriteSheet(file_name, r, c) } {
	text = "";
	image = r == 0 ? AssetManager::GetTexture(file_name) : nullptr;
} // Ctor

ImageUI::~ImageUI() {} // Empty dtor

/*!
* @brief Update the current image animation (if it's a sprite)
* @param dt - The delta time
*/
void ImageUI::Update(const f32& dt) {
	if (!active)
		return;

	BaseUI::Update(dt);
	if (!sprite) 
		return;

	// Update sprite animation
	if ((animationTimer += dt) > (animationFrame / (sprite->Columns() * sprite->Rows()))) {
		animationTimer = 0.f;
		if (++currentCol > sprite->Columns()) {
			currentCol = 0;
			if (++currentRow > sprite->Rows()) {
				currentRow = 0;
			}
		}
	}
}

/*!
* @brief Render the image to the screen
*/
void ImageUI::Render() {
	if (!active)
		return;

	// Prioritize sprite rendering
	if (sprite) {
		sprite->Render(transform, color, currentRow, currentCol);
	}

	if (image && image->data) {
		BaseUI::Render();
		return;
	}
}

/*!
* @brief Get the sprite duration
* @return The sprite duration (in seconds)
*/
f32 ImageUI::GetSpriteDuration() const {
	return animationFrame;
}

/*!
* @brief Set the sprite duration
* @param dur - The sprite duration (in seconds)
*/
void ImageUI::SetSpriteDuration(f32 dur) {
	animationFrame = dur;
}
