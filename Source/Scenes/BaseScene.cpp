#include "BaseScene.hpp"

BaseScene::BaseScene() : scene_entities(0) {
}

BaseScene::~BaseScene() {

}

void BaseScene::PreUpdate(const f32& dt) {
	for (auto& entity : scene_entities) {
		entity->PreUpdate(dt);
	}
}

void BaseScene::Update(const f32& dt) {
	for (auto& entity : scene_entities) {
		entity->Update(dt);
	}
}

void BaseScene::PostUpdate(const f32& dt) {
	for (auto& entity : scene_entities) {
		entity->PostUpdate(dt);
	}
}

void BaseScene::Render() {
	for (auto& entity : scene_entities) {
		entity->Render();
	}
}

void BaseScene::End() {

}