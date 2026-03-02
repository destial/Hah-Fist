#pragma once
#ifndef __SPLASHSCREEN_H
#define __SPLASHSCREEN_H
#include "BaseScene.hpp"

class SplashScreen : public BaseScene {
public:
	SplashScreen();
	~SplashScreen();
	virtual void Init();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	virtual void End();
};

#endif