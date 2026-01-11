#include "EventManager.hpp"

EventManager* EventManager::instance = nullptr;

EventManager::EventManager() {}

EventManager::~EventManager() {}

EventManager* EventManager::GetInstance() {
	if (!instance) {
		instance = new EventManager();
	}
	return instance;
}

void EventManager::Free() {
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

void EventManager::Subscribe(const char* type, Listener listener) {
	listeners[type].push_back(listener);
}

void EventManager::Post(const BaseEvent& event) const {
	auto type = event.GetType();
	if (listeners.find(type) == listeners.end())
		return;

	auto&& tags = listeners.at(type);
	for (auto&& func : tags) {
		func(event);
	}
}

/*
To listen events from the EventManager:
void Start()
{
	EventManager::GetInstance()->Subscribe("ExampleEvent", [](const BaseEvent& ev) {
				
	});
}

To send events to the EventManager:
void Send()
{
	ExampleEvent event;
	EventManager::GetInstance()->Post(event);
}
*/