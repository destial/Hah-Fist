#pragma once
#ifndef __CREDITS_SCENE_H
#define __CREDITS_SCENE_H
#include "BaseScene.hpp"

class CreditsScene : public BaseScene {
public:
	CreditsScene();
	~CreditsScene();
	virtual void Init();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	virtual void End();
};

#endif