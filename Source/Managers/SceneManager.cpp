#include "SceneManager.hpp"
#include "../Scenes/GameScene.hpp"
#include "../Scenes/StartMenuScene.hpp"

namespace DebugUtils {
	void _RenderAll();
}

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager() : all_scenes(), next_scene(Scenes::INIT), current_scene(Scenes::INIT) {
	instance = this;
	all_scenes[Scenes::MAIN_MENU] = new StartMenuScene();
	all_scenes[Scenes::GAME] = new GameScene();
}

SceneManager::~SceneManager() {
	for (int i = 0; i < Scenes::LAST; ++i) {
		if (all_scenes[i]) {
			delete all_scenes[i];
		}
	}
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
		if (all_scenes[next_scene])
			all_scenes[next_scene]->Init();
	}

	current_scene = next_scene;

	if (all_scenes[current_scene])
		all_scenes[current_scene]->PreUpdate(dt);
}

void SceneManager::Update(const f32& dt) {
	if (all_scenes[current_scene])
		all_scenes[current_scene]->Update(dt);
}

void SceneManager::PostUpdate(const f32& dt) {
	if (all_scenes[current_scene])
		all_scenes[current_scene]->PostUpdate(dt);
}

void SceneManager::Render() {
	if (all_scenes[current_scene])
		all_scenes[current_scene]->Render();

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

SceneManager* SceneManager::GetInstance() {
	return instance;
}
