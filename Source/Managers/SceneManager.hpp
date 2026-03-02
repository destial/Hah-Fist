#pragma once
#ifndef __SCENEMANAGER_H
#define __SCENEMANAGER_H
#include "AETypes.h"
#include "../Scenes/BaseScene.hpp"
#include "../Utils/Singleton.hpp"
#include "LevelEditor.hpp"

namespace Scenes {
	typedef enum SceneState {
		INIT = -1,
		SPLASH_SCREEN = 0,
		MAIN_MENU,
		GAME,
		LAST,
	} SceneState;
}

class SceneManager : public Singleton<SceneManager> {
protected:
	BaseScene* all_scenes[Scenes::LAST];

	Scenes::SceneState next_scene;
	Scenes::SceneState current_scene;

	LevelEditor* editor;

public:
	SceneManager();
	~SceneManager();

	// Called before InputHandler
	void PreUpdate(const f32& dt);

	// Called after InputHandler
	void Update(const f32& dt);
	void PostUpdate(const f32& dt);

	// Called after every Update functions
	void Render();

	void SetNextScene(Scenes::SceneState next);
	BaseScene* GetCurrentScene() const;
	Scenes::SceneState GetCurrentState() const;
};

#endif