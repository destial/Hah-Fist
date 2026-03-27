/*!
* @file ButtonUI.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 13 January 2026
* @course CSD1451
* @brief Declaration file for a button UI element
*/

#pragma once
#ifndef __BUTTONUI_H
#define __BUTTONUI_H
#include "BaseUI.hpp"
#include <functional>
#include <vector>

/*!
* @brief A class for a button UI element
*/
class ButtonUI : public BaseUI {
protected:
	// Vectors of function object listeners
	std::vector<std::function<void(MouseButton)>> click_listeners;
	std::vector<std::function<void()>> hover_listeners;
	std::vector<std::function<void()>> unhover_listeners;

	// Only encourage the use of heap instantiation (pointers)
	ButtonUI(ButtonUI const&) = delete; // Remove copy ctor
	ButtonUI& operator=(ButtonUI const&) = delete; // Remove copy assignment
public:
	ButtonUI(AEVec2 pos = { 0.f, 0.f }); // Ctor
	virtual ~ButtonUI(); // Dtor

	/*!
	* @brief Inherited: Update the UI
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief Inherited: Render the UI to the screen
	*/
	virtual void Render();

	/*!
	* @brief Add a listener to this button when clicked by a mouse button
	* @param func - The function listener object
	*/
	void AddClickListener(std::function<void(MouseButton)> func);

	/*!
	* @brief Add a listener to this button when hovered over
	* @param func - The function listener object
	*/
	void AddHoverListener(std::function<void()> func);

	/*!
	* @brief Add a listener to this button when unhovered
	* @param func - The function listener object
	*/
	void AddStopHoverListener(std::function<void()> func);

	/*!
	* @brief Function to call when this button was clicked on
	* @param mousePos - The mouse position
	* @param button - The button clicked
	*/
	void OnMouseClick(AEVec2 mousePos, MouseButton button);

	/*!
	* @brief Function to call when this button is hovered over
	* @param pos - The mouse position
	*/
	void OnMouseHover(AEVec2 pos);

	/*!
	* @brief Function to call when this button was unhovered over
	*/
	void OnMouseStopHover();

	/*!
	* @brief Function to check if mouse is on this button
	* @brief Differs between Circle and Rectangle
	* @param mouse - The mouse world position
	* @return If the mouse is on this button
	*/
	virtual bool IsOn(AEVec2 const& mouse);
};

#endif