#pragma once
#ifndef __MAINMENUSCENE_H
#define __MAINMENUSCENE_H
#include "BaseScene.hpp"

class MainMenuScene : public BaseScene {
public:
	MainMenuScene();
	~MainMenuScene();
	virtual void Init();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	virtual void End();
};

#endif
