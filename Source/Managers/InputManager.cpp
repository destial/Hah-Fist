#include "InputManager.hpp"
#include "AEEngine.h"
#include "../Events/InputEvent.hpp"

InputHandler* InputHandler::instance = nullptr;

InputHandler::InputHandler() {

}

InputHandler::~InputHandler() {

}

InputHandler* InputHandler::GetInstance() {
	if (instance == nullptr) {
		instance = new InputHandler();
	}
	return instance;
}

void InputHandler::Update(const f32& dt) {
	std::vector<u8> keys_t;
	std::vector<u8> keys_c;
	std::vector<u8> keys_r;
	std::vector<u8> keys_p;
	for (u8 i = 0; i < 0xFF; ++i) {
		if (AEInputCheckTriggered(i)) {
			keys_t.push_back(i);
		}
		if (AEInputCheckCurr(i)) {
			keys_c.push_back(i);
		}
		if (AEInputCheckPrev(i)) {
			keys_p.push_back(i);
		}
		if (AEInputCheckReleased(i)) {
			keys_r.push_back(i);
		}
	}
	InputEvent event(keys_t, keys_p, keys_c, keys_r);
	for (auto& fn : InputEvent::Listeners) {
		fn(event);
	}
}