#pragma once
#ifndef __INPUTEVENT_H
#define __INPUTEVENT_H

#include "AEEngine.h"
#include <vector>
#include <functional>

class InputEvent {
private:
	const std::vector<u8>& keys_triggered;
	const std::vector<u8>& keys_prev;
	const std::vector<u8>& keys_current;
	const std::vector<u8>& keys_released;
public:
	static std::vector<std::function<void(const InputEvent&)>> Listeners;
	InputEvent(
		const std::vector<u8>& keys_t,
		const std::vector<u8>& keys_p,
		const std::vector<u8>& keys_c,
		const std::vector<u8>& keys_r)
		: 
		keys_triggered(keys_t),
		keys_prev(keys_p),
		keys_current(keys_c),
		keys_released(keys_r) {}
	~InputEvent() {}

	const std::vector<u8>& GetKeysTriggered() const {
		return keys_triggered;
	}

	const std::vector<u8>& GetKeysHeld() const {
		return keys_triggered;
	}

	const std::vector<u8>& GetKeysReleased() const {
		return keys_released;
	}

	const std::vector<u8>& GetKeysPrevious() const {
		return keys_prev;
	}
};

/*
Usage:

void Start() {
	auto OnKeyPress = [](const InputEvent& ev) {
		for (auto key : ev.GetKeysTriggered()) {
			if (key == ...check key here...) {
				...function here...
			}

			...you can also check for other keys...
		}
	};

	InputEvent::Listeners.push_back(OnGameExit);
}
*/

#endif