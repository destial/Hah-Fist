#include "BaseScene.hpp"
#include <algorithm>
#include <iostream>

BaseScene::BaseScene() : scene_entities(0), particleSystem(new ParticleSystem) {
}

BaseScene::~BaseScene() {
	scene_entities.clear();
	delete particleSystem;
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
	particleSystem->Update(dt);
}

void BaseScene::PostUpdate(const f32& dt) {
	for (auto& entity : scene_entities) {
		entity->PostUpdate(dt);
	}
}

static bool compare(BaseEntity* a, BaseEntity* b) {
	return b->layer > a->layer;
}

void BaseScene::Render() {
	std::stable_sort(scene_entities.begin(), scene_entities.end(), compare);

	for (auto& entity : scene_entities) {
		entity->Render();
	}
	particleSystem->Render();
}

void BaseScene::End() {
	for (auto& entity : scene_entities) {
		delete entity;
	}
	scene_entities.clear();
	particleSystem->Clear();
	std::cout << "Scene ended\n";
}