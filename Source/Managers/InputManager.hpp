#pragma once
#ifndef __INPUTMANAGER_H
#define __INPUTMANAGER_H
#include "AETypes.h"

class InputHandler {
private:
	static InputHandler* instance;
	InputHandler();
	~InputHandler();
public:
	static InputHandler* GetInstance();
	static void Free();
	void Update(const f32& dt);
};

#endif