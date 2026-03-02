#include "SceneManager.hpp"
#include "../Scenes/GameScene.hpp"
#include "../Scenes/SplashScreen.hpp"
#include "../Scenes/StartMenuScene.hpp"
#include "../UI/Debug.hpp"

namespace DebugUtils {
	void _RenderAll();
}

SceneManager::SceneManager() : all_scenes{}, next_scene{ Scenes::INIT }, current_scene{ Scenes::INIT }, editor{nullptr} {
	instance = this;
	all_scenes[Scenes::SPLASH_SCREEN] = new SplashScreen();
	all_scenes[Scenes::MAIN_MENU] = new StartMenuScene();
	all_scenes[Scenes::GAME] = new GameScene();
	editor = new LevelEditor{ all_scenes[0] };
}

SceneManager::~SceneManager() {
	for (int i = 0; i < Scenes::LAST; ++i) {
		if (all_scenes[i]) {
			delete all_scenes[i];
		}
	}
	delete editor;
}

void SceneManager::PreUpdate(const f32& dt) {
	if (current_scene != next_scene) {
		if (current_scene != Scenes::INIT) {
			if (all_scenes[current_scene])
				all_scenes[current_scene]->End();
		}

		if (next_scene == Scenes::INIT) {
			return;
		}
		if (all_scenes[next_scene]) {
			all_scenes[next_scene]->Init();
			editor->SetScene(all_scenes[next_scene]);
		}
	}

	current_scene = next_scene;

	if (all_scenes[current_scene] && !editor->IsToggled()) {
		all_scenes[current_scene]->PreUpdate(dt);
	}
}

void SceneManager::Update(const f32& dt) {
	if (AEInputCheckTriggered(AEVK_P)) {
		editor->Toggle();
	}

	if (AEInputCheckTriggered(AEVK_G)) {
		DebugUtils::ToggleRender(!DebugUtils::IsRendering());
	}

	if (all_scenes[current_scene]) {
		if (!editor->IsToggled()) {
			all_scenes[current_scene]->Update(dt);
		}
		else {
			editor->Update(dt);
		}
	}
}

void SceneManager::PostUpdate(const f32& dt) {
	if (all_scenes[current_scene] && !editor->IsToggled())
		all_scenes[current_scene]->PostUpdate(dt);
}

void SceneManager::Render() {
	if (all_scenes[current_scene]) {
		all_scenes[current_scene]->Render();
		if (editor->IsToggled()) {
			editor->Render();
		}
	}

	DebugUtils::_RenderAll();
}

void SceneManager::SetNextScene(Scenes::SceneState next) {
	this->next_scene = next;
}

BaseScene* SceneManager::GetCurrentScene() const {
	return all_scenes[current_scene];
}

Scenes::SceneState SceneManager::GetCurrentState() const {
	return current_scene;
}