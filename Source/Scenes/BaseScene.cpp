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
	if (physicsManager != nullptr)
	{
		physicsManager->PreUpdate(dt);
	}
}

void BaseScene::Update(const f32& dt) {
	for (auto& entity : scene_entities) {
		entity->Update(dt);
	}
	particleSystem->Update(dt);
	if (physicsManager != nullptr)
	{
		physicsManager->Update(dt);
	}
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
	if (physicsManager != nullptr)
	{
		physicsManager->Render();
	}
}

void BaseScene::End() {
	for (auto& entity : scene_entities) {
		delete entity;
	}
	scene_entities.clear();
	particleSystem->Clear();
	if (physicsManager != nullptr)
	{
		physicsManager->End();
	}
	std::cout << "Scene ended\n";
}

void BaseScene::AddEntityToScene(BaseEntity* entity)
{
	scene_entities.push_back(entity);
	//Checks and creates a new pointer to game object with entity, if the entity derives from gameobject.
	if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(entity))
	{
		if (physicsManager == nullptr)
		{
			physicsManager = new PhysicsManager{};
		}

		physicsManager->PushGameObject(go);
	}
}