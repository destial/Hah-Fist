/*!
* @file InputManager.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 12 January 2026
* @course CSD1451
* @brief Definition file for an input manager that handles input events
*/

#include "InputManager.hpp"
#include "AEEngine.h"
#include "AssetManager.hpp"

InputHandler::InputHandler() 
: keys_t(0), keys_c(0), keys_r(0), keys_p(0) {
	// initialize input event pointer
	this->event = new InputEvent{ keys_t, keys_p, keys_c, keys_r };
	this->EndFrame();
}
InputHandler::~InputHandler() {
	// clear all listeners
	InputEvent::Listeners.clear();

	// we called new in the ctor, so we delete here
	delete event;
}

void InputHandler::Update(const f32& dt) {
	// clear all currently pressed keys
	keys_c.clear();
	for (u8 i = 0; i < 0xFF; ++i) { // loop through every key
		if (AEInputCheckCurr(i)) {
			keys_c.push_back(i); // add currently pressed keys
		}
		if (AEInputCheckTriggered(i)) {
			bool already = false;
			for (auto& k : keys_t) {
				if (k == i) {
					already = true; // dont add if already inside
					break;
				}
			}
			if (!already)
				keys_t.push_back(i); // add currently triggered keys
		}
		if (AEInputCheckPrev(i)) {
			bool already = false;
			for (auto& k : keys_p) {
				if (k == i) {
					already = true; // dont add if already inside
					break;
				}
			}
			if (!already)
				keys_p.push_back(i); // add currently previous keys
		}
		if (AEInputCheckReleased(i)) {
			bool already = false;
			for (auto& k : keys_r) {
				if (k == i) {
					already = true; // dont add if already inside
					break;
				}
			}
			if (!already)
				keys_r.push_back(i); // add currently released keys
		}
	}

	if (!keys_t.empty() && !keys_p.empty() && !keys_c.empty() && !keys_r.empty())
		return; // dont dispatch event if no input was handled

	for (auto& pair : InputEvent::Listeners) {
		for (auto& fn : pair.second) {
			fn(event); // dispatch event to registered listeners
		}
	}
}

void InputHandler::EndFrame() {
	// clear the key vectors
	keys_t.clear();
	keys_p.clear();
	keys_r.clear();
}

// Global InputListener, uses the InputHandler instance as the owner
InputEvent::InputListeners& operator+= (InputEvent::InputListeners& lhs, InputEvent::InputListener rhs) {
	return InputEvent::Listeners += std::pair<void*, InputEvent::InputListener>{InputHandler::GetInstance(), rhs};
}