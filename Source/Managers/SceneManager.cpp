#include "SceneManager.hpp"

SceneManager::SceneManager() : next_scene(0), current_scene(0) {
}

SceneManager::~SceneManager() {
}

void SceneManager::PreUpdate(const f32& dt) {
	if (current_scene != next_scene) {
		if (current_scene)
			current_scene->End();

		if (next_scene)
			next_scene->Init();
	}

	current_scene = next_scene;

	if (current_scene)
		current_scene->PreUpdate(dt);
}

void SceneManager::Update(const f32& dt) {
	if (current_scene)
		current_scene->Update(dt);
}

void SceneManager::PostUpdate(const f32& dt) {
	if (current_scene)
		current_scene->PostUpdate(dt);
}

void SceneManager::Render() {
	if (current_scene)
		current_scene->Render();
}

void SceneManager::SetNextScene(BaseScene* next) {
	this->next_scene = next;
}

BaseScene* SceneManager::GetCurrentScene() const {
	return current_scene;
}
