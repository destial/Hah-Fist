#pragma once
#ifndef __SCENEMANAGER_H
#define __SCENEMANAGER_H
#include "AETypes.h"
#include "../Scenes/BaseScene.h"

class SceneManager {
protected:
	BaseScene* current_scene;

public:
	SceneManager();
	~SceneManager();

	void Update(const f32& dt);
	void Render();

	void SetNextScene(BaseScene* next);
	const BaseScene* GetCurrentScene();
};

#endif