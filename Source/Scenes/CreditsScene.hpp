#pragma once
#ifndef __CREDITS_SCENE_H
#define __CREDITS_SCENE_H
#include "BaseScene.hpp"

class CreditsScene : public BaseScene {
public:
	CreditsScene();
	~CreditsScene();
	virtual void Init();
	virtual void Update(const f32& dt);
};

#endif