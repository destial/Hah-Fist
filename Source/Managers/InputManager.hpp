#pragma once
#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H
#include "AETypes.h"
#include <vector>
#include "../Events/InputEvent.hpp"

class InputHandler {
private:
	static InputHandler* instance;
	InputHandler();
	~InputHandler();
	std::vector<u8> keys_t;
	std::vector<u8> keys_c;
	std::vector<u8> keys_r;
	std::vector<u8> keys_p;
	InputEvent* event;
public:
	static InputHandler* GetInstance();
	static void Free();
	void Update(const f32& dt);
};

#endif