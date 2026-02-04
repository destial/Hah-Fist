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

InputEvent::InputListeners& operator+= (InputEvent::InputListeners& lhs, std::pair<void*, InputEvent::InputListener> rhs) {
	if (lhs.count(rhs.first) == 0) {
		lhs[rhs.first] = std::vector<InputEvent::InputListener>(0);
	}
	lhs[rhs.first].push_back(rhs.second);
	return lhs;
}

InputEvent::InputListeners& operator-= (InputEvent::InputListeners& lhs, std::pair<void*, InputEvent::InputListener> rhs) {
	for (InputEvent::InputListeners::iterator it = lhs.begin(); it != lhs.end();) {
		auto& pair = *it;
		for (std::vector<InputEvent::InputListener>::iterator vit = pair.second.begin(); vit != pair.second.end();) {
			if ((*vit) == rhs.second) {
				vit = pair.second.erase(vit);
			}
			else {
				++vit;
			}
		}
		
	}
	return lhs;
}

InputEvent::InputListeners& operator-= (InputEvent::InputListeners& lhs, void* ptr) {
	if (lhs.count(ptr) != 0) {
		lhs[ptr].clear();
		lhs.erase(ptr);
	}
	return lhs;
}
