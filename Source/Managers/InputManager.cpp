#include "InputManager.hpp"
#include "AEEngine.h"

InputHandler* InputHandler::instance = nullptr;

InputHandler::InputHandler() : keys_t(0), keys_c(0), keys_r(0), keys_p(0) {
	this->event = new InputEvent(keys_t, keys_p, keys_c, keys_r);
}
InputHandler::~InputHandler() {
	delete event;
}

InputHandler* InputHandler::GetInstance() {
	if (instance == nullptr) {
		instance = new InputHandler();
	}
	return instance;
}

void InputHandler::Free() {
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

void InputHandler::Update(const f32& dt) {
	keys_t.clear();
	keys_p.clear();
	keys_r.clear();
	keys_c.clear();
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

	if (!keys_t.empty() && !keys_p.empty() && !keys_c.empty() && !keys_r.empty())
		return;

	for (auto& fn : InputEvent::Listeners) {
		fn(event);
	}
}