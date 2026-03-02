#include "InputManager.hpp"
#include "AEEngine.h"
#include "AssetManager.hpp"

InputHandler::InputHandler() : keys_t(0), keys_c(0), keys_r(0), keys_p(0) {
	this->event = new InputEvent(keys_t, keys_p, keys_c, keys_r);
	this->EndFrame();
}
InputHandler::~InputHandler() {
	InputEvent::Listeners.clear();
	delete event;
}

void InputHandler::Update(const f32& dt) {
	keys_c.clear();
	for (u8 i = 0; i < 0xFF; ++i) {
		if (AEInputCheckCurr(i)) {
			keys_c.push_back(i);
		}
		if (AEInputCheckTriggered(i)) {
			bool already = false;
			for (auto& k : keys_t) {
				if (k == i) {
					already = true;
					break;
				}
			}
			if (!already)
				keys_t.push_back(i);
		}
		if (AEInputCheckPrev(i)) {
			bool already = false;
			for (auto& k : keys_p) {
				if (k == i) {
					already = true;
					break;
				}
			}
			if (!already)
				keys_p.push_back(i);
		}
		if (AEInputCheckReleased(i)) {
			bool already = false;
			for (auto& k : keys_r) {
				if (k == i) {
					already = true;
					break;
				}
			}
			if (!already)
				keys_r.push_back(i);
		}
	}

	if (!keys_t.empty() && !keys_p.empty() && !keys_c.empty() && !keys_r.empty())
		return;

	for (auto& pair : InputEvent::Listeners) {
		for (auto& fn : pair.second) {
			fn(event);
		}
	}
}

void InputHandler::EndFrame() {
	keys_t.clear();
	keys_p.clear();
	keys_r.clear();
}

InputEvent::InputListeners& operator+= (InputEvent::InputListeners& lhs, InputEvent::InputListener rhs) {
	return InputEvent::Listeners += std::pair<void*, InputEvent::InputListener>{InputHandler::GetInstance(), rhs};
}