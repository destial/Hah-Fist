#include "BaseScene.hpp"
#include <algorithm>
#include <memory>

BaseScene::BaseScene() : scene_entities(0) {
}

BaseScene::~BaseScene() {
	scene_entities.clear();
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

static bool compare(std::shared_ptr<BaseEntity> a, std::shared_ptr<BaseEntity> b) {
	return b->layer > a->layer;
}

void BaseScene::Render() {
	std::stable_sort(scene_entities.begin(), scene_entities.end(), compare);

	for (auto& entity : scene_entities) {
		entity->Render();
	}
}

void BaseScene::End() {
	scene_entities.clear();
}