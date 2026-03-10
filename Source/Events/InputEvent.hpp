/*!
* @file InputEvent.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 12 January 2026
* @course CSD1451
* @brief Declaration file for an input event that stores processed input data
*/

#pragma once
#ifndef __INPUTEVENT_H
#define __INPUTEVENT_H
#include "AEEngine.h"
#include <vector>
#include <map>
#include <functional>

/*!
* @brief Stores data about what keys are triggered this frame, currently held, released,
* @brief and the keys on the previous frame. There can only be one instance of this which
* @brief is found inside InputHandler singleton class.
*/
class InputEvent {
private:
	const std::vector<u8>& keys_triggered; // Keys triggered this frame
	const std::vector<u8>& keys_prev; // Keys on the previous frame
	const std::vector<u8>& keys_current; // Keys this frame
	const std::vector<u8>& keys_released; // Keys released this frame
public:
	typedef std::function<void(const InputEvent*)> InputListener;
	typedef std::map<void*, std::vector<InputListener>> InputListeners;
	static InputListeners Listeners; // Global valid InputListeners
	InputEvent(
		const std::vector<u8>& keys_t,
		const std::vector<u8>& keys_p,
		const std::vector<u8>& keys_c,
		const std::vector<u8>& keys_r); // Ctor
	InputEvent(InputEvent const&) = delete; // Remove copy ctor
	InputEvent& operator=(InputEvent const&) = delete; // Remove copy assignment
	~InputEvent(); // Dtor

	/*!
	* @brief Get keys triggered this frame
	* @return The vector of keys triggered this frame
	*/
	const std::vector<u8>& GetKeysTriggered() const;

	/*!
	* @brief Get keys held this frame
	* @return The vector of keys held this frame
	*/
	const std::vector<u8>& GetKeysHeld() const;

	/*!
	* @brief Get keys released this frame
	* @return The vector of keys released this frame
	*/
	const std::vector<u8>& GetKeysReleased() const;

	/*!
	* @brief Get previous keys this frame
	* @return The vector of previous keys this frame
	*/
	const std::vector<u8>& GetKeysPrevious() const;

	/*!
	* @brief Check if this key is triggered this frame
	* @param key - The key to check (e.g. AEVK_A)
	*/
	const bool IsKeyTriggered(u8 key) const;

	/*!
	* @brief Check if this key is held this frame
	* @param key - The key to check (e.g. AEVK_A)
	*/
	const bool IsKeyHeld(u8 key) const;

	/*!
	* @brief Check if this key is released this frame
	* @param key - The key to check (e.g. AEVK_A)
	*/
	const bool IsKeyReleased(u8 key) const;
};

/*!
* Comparator to check similar listeners
*/
bool operator== (InputEvent::InputListener& lhs, const InputEvent::InputListener& rhs);

/*!
* Neater way of adding an input listener with the global owner
*/
InputEvent::InputListeners& operator+= (InputEvent::InputListeners& lhs, InputEvent::InputListener rhs);

/*!
* Neater way of adding an input listener with an owner
*/
InputEvent::InputListeners& operator+= (InputEvent::InputListeners& lhs, std::pair<void*, InputEvent::InputListener> rhs);

/*!
* Neater way of removing an input listener with an owner
*/
InputEvent::InputListeners& operator-= (InputEvent::InputListeners& lhs, std::pair<void*, InputEvent::InputListener> rhs);

/*!
* Neater way of removing all input listeners from a specific owner
*/
InputEvent::InputListeners& operator-= (InputEvent::InputListeners& lhs, void* ptr);

#endif