#include "InputEvent.hpp"

std::vector<std::function<void(const InputEvent*)>> InputEvent::Listeners;

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