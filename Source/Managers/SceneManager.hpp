#pragma once
#ifndef __SCENEMANAGER_H
#define __SCENEMANAGER_H
#include "AETypes.h"
#include "../Scenes/BaseScene.hpp"

class SceneManager {
protected:
	BaseScene* current_scene;
	BaseScene* prev_scene;

public:
	SceneManager();
	~SceneManager();

	void PreUpdate(const f32& dt);
	void Update(const f32& dt);
	void PostUpdate(const f32& dt);
	void Render();

	void SetNextScene(BaseScene* next);
	const BaseScene* GetCurrentScene();
};

#endif