/*!
* @file InputManager.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 12 January 2026
* @course CSD1451
* @brief Declaration file for an input manager that handles input events
*/

#pragma once
#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H
#include "AETypes.h"
#include <vector>
#include "../Events/InputEvent.hpp"
#include "../Utils/Singleton.hpp"

/*!
* @brief Singleton class that handles input events
*/
class InputHandler : public Singleton<InputHandler> {
private:
	std::vector<u8> keys_t; // All keys in this frame that were triggered
	std::vector<u8> keys_c; // All keys in this frame that are pressed
	std::vector<u8> keys_r; // All keys in this frame that were released
	std::vector<u8> keys_p; // All keys in this frame that were pressed in the previous frame
	InputEvent* event; // The main event pointer, should not be deleted until the application ends
public:
	InputHandler(); // Ctor
	~InputHandler(); // Dtor

	/*!
	* @brief Update the InputHandler each frame
	*/
	void Update(const f32& dt);

	/*!
	* @brief Tell the InputHandler that this frame has ended
	*/
	void EndFrame();
};

#endif