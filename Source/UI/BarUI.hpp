/*!
* @file BarUI.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 9 March 2026
* @course CSD1451
* @brief Declaration file for a slider bar UI element
*/

#pragma once
#ifndef __BARUI_H
#define __BARUI_H
#include "BaseUI.hpp"

/*!
* @brief A class for a slider bar UI element
* @brief Can be interactive or not, represents a value between 0-1
*/
class BarUI : public BaseUI {
protected:
	bool mouse_dragged; // If this slider is being dragged
	f32 value; // A value between 0-1
	AEMtx33 overlay_transform; // The actual value overlay transform
public:
	// Only encourage the use of heap instantiation (pointers)
	BarUI(AEVec2 pos = { 0.f, 0.f });
	BarUI(BarUI const&) = delete; // Remove copy ctor
	BarUI& operator=(BarUI const&) = delete; // Remove copy assignment
	virtual ~BarUI(); // Dtor

	/*!
	* @brief Inherited: Update the UI after input has been processed
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief Inherited: Post-update the UI after everything else has been processed
	*/
	virtual void PostUpdate(const f32& dt);
	
	/*!
	* @brief Inherited: Render the UI to the screen
	*/
	virtual void Render();

	/*!
	* @brief Get the value of the slider
	* @return A value between 0-1
	*/
	f32 GetValue() const;

	/*!
	* @brief Set the value of the slider
	* @param v - A value between 0-1
	*/
	virtual void SetValue(f32 v);

	/*!
	* @brief Get the state of dragging of this slider
	* @return The mouse drag state
	*/
	bool IsDragging() const;
};

#endif