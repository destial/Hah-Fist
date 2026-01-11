#ifndef __BASEEVENT_H
#define __BASEEVENT_H
#include <string>

class BaseEvent {
protected:
	BaseEvent() {}
public:
	virtual ~BaseEvent() {}
	virtual const char* GetType() const = 0;
};

/*
Declare an event class like this:

#include "BaseEvent.hpp"

class ExampleEvent : public BaseEvent {
public:
	ExampleEvent();
	~ExampleEvent();

	static constexpr const char* type = "ExampleEvent";
	virtual const char* GetType() const {
		return "ExampleEvent";
	}

	// here you can add anything to pass to the event handler
	AEVec2 position;
	BaseEntity* entity;
	AEMtx33 matrix;
	// etc...
};
*/

#endif