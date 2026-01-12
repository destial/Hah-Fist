#include "SceneManager.hpp"

SceneManager::SceneManager() : current_scene(0), prev_scene(0) {
}

SceneManager::~SceneManager() {
}

void SceneManager::PreUpdate(const f32& dt) {
	if (prev_scene != current_scene) {
		if (prev_scene)
			prev_scene->End();

		if (current_scene)
			current_scene->Init();
	}

	prev_scene = current_scene;

	if (prev_scene)
		prev_scene->PreUpdate(dt);
}

void SceneManager::Update(const f32& dt) {
	if (prev_scene)
		prev_scene->Update(dt);
}

void SceneManager::PostUpdate(const f32& dt) {
	if (prev_scene)
		prev_scene->PostUpdate(dt);
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
