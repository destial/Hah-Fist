#pragma once
#ifndef __INPUTEVENT_H
#define __INPUTEVENT_H

#include "../Utils/BaseEvent.hpp"
#include "AETypes.h"
#include "AEInput.h"

class InputEvent : public BaseEvent {
protected:
	const f32 dt;
	const int key;
public:
	InputEvent(const f32& dt, const int key) : dt(dt), key(key) {}
	virtual ~InputEvent() {}

	static constexpr const char* type = "InputEvent";
	virtual const char* GetType() const {
		return type;
	}

	const f32& GetDt() const {
		return dt;
	}

	const int GetKeyTriggered() const {
		return key;
	}
};

#endif