#include "BaseScene.hpp"
#include <algorithm>
#include <iostream>

BaseScene::BaseScene() : scene_entities(0), particleSystem(new ParticleSystem) {
}

BaseScene::~BaseScene() {
	scene_entities.clear();
	delete particleSystem;
	if (physicsManager)
		delete physicsManager;
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
	if (physicsManager) {
		physicsManager->Clear();
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

void BaseScene::RemoveEntityFromScene(BaseEntity* entity)
{
	bool deleted = false;
	for (std::vector<BaseEntity*>::iterator it = scene_entities.begin(); it != scene_entities.end(); it++) {
		if (entity == *it) {
			scene_entities.erase(it);
			deleted = true;
			break;
		}
	}

	GameObjectEntity* go;
	if (physicsManager && (go = dynamic_cast<GameObjectEntity*>(entity))) {
		for (std::vector<GameObjectEntity*>::iterator it = physicsManager->gameObjects.begin(); it != physicsManager->gameObjects.end(); it++) {
			if (*it == go) {
				physicsManager->gameObjects.erase(it);
				break;
			}
		}
	}

	if (deleted) {
		delete entity;
	}
}

std::vector<BaseEntity*> const& BaseScene::Entities() const {
	return scene_entities;
}

template<typename E> E* BaseScene::GetFirstEntityOfType() const {
	static_assert(std::is_base_of<BaseEntity, E>::value, "E must derive from BaseEntity!");
	for (BaseEntity* en : scene_entities) {
		if (E* first = dynamic_cast<E*>(en)) {
			return first;
		}
	}
}

template<typename E> std::vector<E*> BaseScene::GetEntitesOfType() const {
	static_assert(std::is_base_of<BaseEntity, E>::value, "E must derive from BaseEntity!");
	std::vector<E*> vect;
	for (BaseEntity* en : scene_entities) {
		if (E* type = dynamic_cast<E*>(en)) {
			vect.push_back(type);
		}
	}
	return vect;
}
