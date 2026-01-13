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
	typedef std::function<void(const InputEvent*)> InputListener;
	static std::vector<InputListener> Listeners;
	InputEvent(
		const std::vector<u8>& keys_t,
		const std::vector<u8>& keys_p,
		const std::vector<u8>& keys_c,
		const std::vector<u8>& keys_r);
	~InputEvent();

	const std::vector<u8>& GetKeysTriggered() const;
	const std::vector<u8>& GetKeysHeld() const;
	const std::vector<u8>& GetKeysReleased() const;
	const std::vector<u8>& GetKeysPrevious() const;

	const bool IsKeyTriggered(u8 key) const;
	const bool IsKeyHeld(u8 key) const;
	const bool IsKeyReleased(u8 key) const;
};

bool operator== (InputEvent::InputListener& lhs, InputEvent::InputListener& rhs);
std::vector<InputEvent::InputListener>& operator+= (std::vector<InputEvent::InputListener>& lhs, InputEvent::InputListener& rhs);
std::vector<InputEvent::InputListener>& operator+= (std::vector<InputEvent::InputListener>& lhs, InputEvent::InputListener rhs);
std::vector<InputEvent::InputListener>& operator-= (std::vector<InputEvent::InputListener>& lhs, InputEvent::InputListener& rhs);
std::vector<InputEvent::InputListener>& operator-= (std::vector<InputEvent::InputListener>& lhs, InputEvent::InputListener rhs);

#endif