#pragma once
#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H
#include "AETypes.h"
#include <vector>
#include "../Events/InputEvent.hpp"
#include "../Utils/Singleton.hpp"

class InputHandler : public Singleton<InputHandler> {
private:
	std::vector<u8> keys_t;
	std::vector<u8> keys_c;
	std::vector<u8> keys_r;
	std::vector<u8> keys_p;
	InputEvent* event;
public:
	InputHandler();
	~InputHandler();
	void Update(const f32& dt);
	void EndFrame();
};

#endif