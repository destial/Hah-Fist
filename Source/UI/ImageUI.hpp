/*!
* @file ImageUI.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 10 March 2026
* @course CSD1451
* @brief Declaration file for an image UI element
*/

#pragma once
#ifndef __IMAGEUI_H
#define __IMAGEUI_H
#include "BaseUI.hpp"
#include "../Items/Image.hpp"
#include "../Items/SpriteSheet.hpp"

/*!
* @brief A class that holds image data meant for UI
*/
class ImageUI : public BaseUI {
protected:
	SpriteSheet* sprite; // Sprite animation data
public:
	f32 animation_timer, animation_frame; // Sprite animation data
	int current_row, current_col; // Sprite animation data

	ImageUI(const char* file_name, AEVec2 pos = { 0.f, 0.f }, int rows = 0, int cols = 0); // Ctor
	ImageUI(ImageUI const&) = delete; // Remove copy ctor
	ImageUI& operator=(ImageUI const&) = delete; // Remove copy assignment
	virtual ~ImageUI(); // Dtor

	/*!
	* @brief Update the current image animation (if it's a sprite)
	* @param dt - The delta time
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief Render the image to the screen
	*/
	virtual void Render();

	/*!
	* @brief Get the sprite duration
	* @return The sprite duration (in seconds)
	*/
	f32 GetSpriteDuration() const;

	/*!
	* @brief Set the sprite duration
	* @param dur - The sprite duration (in seconds)
	*/
	void SetSpriteDuration(f32 dur);
};

#endif