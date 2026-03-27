/*!
* @file CircleButtonUI.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 13 January 2026
* @course CSD1451
* @brief Declaration file for a circular button UI element
*/

#pragma once
#ifndef __CIRCLEBUTTONUI_H
#define __CIRCLEBUTTONUI_H
#include "ButtonUI.hpp"

/*!
* @brief A class for a circular button UI element
*/
class CircleButtonUI : public ButtonUI {
protected:
	// Only encourage the use of heap instantiation (pointers)
	CircleButtonUI(CircleButtonUI const&) = delete; // Remove copy ctor
	CircleButtonUI& operator=(CircleButtonUI const&) = delete; // Remove copy assignment
public:
	f32 radius; // The radius of this button
	CircleButtonUI(AEVec2 pos = { 0.f, 0.f }); // Ctor
	virtual ~CircleButtonUI(); // Dtor

	/*!
	* @brief Inherited: Post-update the UI after everything has been processed
	* @brief Update's the scale based on radius
	*/
	virtual void PostUpdate(const f32& dt);

	/*!
	* @brief Inherited: Function to check if mouse is on this button
	* @brief Differs between Circle and Rectangle
	* @param mouse - The mouse world position
	* @return If the mouse is on this button
	*/
	virtual bool IsOn(AEVec2 const& mouse);
};

#endif