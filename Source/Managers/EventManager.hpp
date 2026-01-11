#pragma once
#ifndef __EVENTMANAGER_H
#define __EVENTMANAGER_H
#include "../Utils/BaseEvent.hpp"
#include <functional>
#include <map>
#include <vector>
#include <string>

typedef std::function<void(const BaseEvent&)> Listener;

class EventManager {
private:
	EventManager();
	~EventManager();
	static EventManager* instance;
	std::map<const char*, std::vector<Listener>> listeners;
public:
	EventManager(const EventManager&) = delete; // ensure no other copies of this can exist
	static EventManager* GetInstance();
	static void Free();

	void Subscribe(const char* type, Listener listener);
	void Post(const BaseEvent& event) const;
};

#endif