#include "SceneManager.hpp"

SceneManager::SceneManager() : current_scene(0), prev_scene(0) {
}

SceneManager::~SceneManager() {
}

void SceneManager::Update(const f32& dt) {
	if (prev_scene != current_scene) {
		if (prev_scene)
			prev_scene->End();

		if (current_scene)
			current_scene->Init();
	}

	prev_scene = current_scene;

	if (current_scene)
		current_scene->Update(dt);
}

void SceneManager::Render() {
	if (prev_scene)
		prev_scene->Render();
}

void SceneManager::SetNextScene(BaseScene* next) {
	this->current_scene = next;
}

const BaseScene* SceneManager::GetCurrentScene() {
	return current_scene;
}
