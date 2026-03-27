/*!
* @file BaseUI.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 13 January 2026
* @course CSD1451
* @brief Declaration file for a base UI element
*/

#pragma once
#ifndef __BASEUI_H
#define __BASEUI_H
#include <string>
#include "../Entities/BaseEntity.hpp"
#include "../Utils/Utils.hpp"
#include "../Items/Image.hpp"

/*!
* @brief The base UI class that should be inherited by all UI classes
*/
class BaseUI : public BaseEntity {
protected:
	bool interactive; // If this element can be interacted with
	bool mouse_hovered; // If the mouse is currently hovered
	bool clicked_this_frame; // If this element was clicked this frame (triggered)

	// Only encourage the use of heap instantiation (pointers)
	BaseUI(AEVec2 pos = { 0.f, 0.f }); // Ctor
	BaseUI(BaseUI const&) = delete; // Remove copy ctor
	BaseUI& operator=(BaseUI const&) = delete; // Remove copy assignment
public:
	virtual ~BaseUI(); // Dtor
	enum MouseButton { // Easier understanding of AEVK macro
		NONE = 0,
		LEFT = AEVK_LBUTTON,
		RIGHT = AEVK_RBUTTON,
		MIDDLE = AEVK_MBUTTON
	};
	enum class TextAlignment : char {
		CENTER,
		LEFT_CORNER,
		RIGHT_CORNER
	} text_alignment; // To signal how the text display should be aligned
	s8 font; // Font id
	std::string text; // The text to display
	f32 text_size; // The text size
	Color text_color; // The text color
	Color overlay_color; // The interactive color
	Color overlay_text_color; // The interactive text color
	Image* overlay_texture; // The interactive texture

	/*!
	* @brief Post-update the UI after everything else has been processed
	*/
	virtual void PostUpdate(const f32& dt);

	/*!
	* @brief Render the UI to the screen
	*/
	virtual void Render();

	/*!
	* @brief Render the UI text to the screen
	*/
	virtual void RenderText();

	/*!
	* @brief Get whether this element is interactive
	* @return This element's interactivity
	*/
	bool IsInteractive() const;

	/*!
	* @brief Set the element's interactivity
	* @param i - The interactive state
	*/
	virtual void SetInteractive(bool i);
};

#endif