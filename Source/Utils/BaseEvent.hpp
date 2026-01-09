#ifndef __BASEEVENT_H
#define __BASEEVENT_H
#include <string>

class BaseEvent {
private:
	BaseEvent() {}
public:
	virtual ~BaseEvent() {}
	virtual std::string GetType() const = 0;
};

/*
Declare an event class like this:

#include "BaseEvent.hpp"

class ExampleEvent : public BaseEvent {
public:
	ExampleEvent();
	~ExampleEvent();
	virtual std::string GetType() const {
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