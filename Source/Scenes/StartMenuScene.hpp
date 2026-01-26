#pragma once
#ifndef __STARTMENUSCENE_H
#define __STARTMENUSCENE_H
#include "BaseScene.hpp"

class StartMenuScene : public BaseScene {
public:
	StartMenuScene();
	~StartMenuScene();
	virtual void Init();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	virtual void End();
};

#endif
