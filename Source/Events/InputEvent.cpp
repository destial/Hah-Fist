#include "InputEvent.hpp"

InputEvent::InputListeners InputEvent::Listeners;

InputEvent::InputEvent(const std::vector<u8>& keys_t, const std::vector<u8>& keys_p, const std::vector<u8>& keys_c, const std::vector<u8>& keys_r)
	:
	keys_triggered(keys_t),
	keys_prev(keys_p),
	keys_current(keys_c),
	keys_released(keys_r) {
}

InputEvent::~InputEvent() {}

const std::vector<u8>& InputEvent::GetKeysTriggered() const {
	return keys_triggered;
}

const std::vector<u8>& InputEvent::GetKeysHeld() const {
	return keys_triggered;
}

const std::vector<u8>& InputEvent::GetKeysReleased() const {
	return keys_released;
}

const std::vector<u8>& InputEvent::GetKeysPrevious() const {
	return keys_prev;
}

const bool InputEvent::IsKeyTriggered(u8 key) const {
	for (auto& k : keys_triggered) {
		if (k == key) {
			return true;
		}
	}
	return false;
}

const bool InputEvent::IsKeyHeld(u8 key) const {
	for (auto& k : keys_current) {
		if (k == key) {
			return true;
		}
	}
	return false;
}

const bool InputEvent::IsKeyReleased(u8 key) const {
	for (auto& k : keys_released) {
		if (k == key) {
			return true;
		}
	}
	return false;
}

bool operator== (InputEvent::InputListener& lhs, const InputEvent::InputListener& rhs) {
	return lhs.target<InputEvent::InputListener>() == rhs.target<InputEvent::InputListener>();
}

InputEvent::InputListeners& operator+= (InputEvent::InputListeners& lhs, InputEvent::InputListener rhs) {
	lhs.push_back(rhs);
	return lhs;
}

InputEvent::InputListeners& operator-= (InputEvent::InputListeners& lhs, InputEvent::InputListener rhs) {
	for (InputEvent::InputListeners::iterator it = lhs.begin(); it != lhs.end();) {
		if (*it == rhs) {
			it = lhs.erase(it);
		}
		else {
			++it;
		}
	}
	return lhs;
}
