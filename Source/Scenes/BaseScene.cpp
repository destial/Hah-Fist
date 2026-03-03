/*!
* @file BaseScene.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @author Brandon Koh (brandonshaohui.koh@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @author Ryan Lau (r.lau@digipen.edu)
* @date 7 January 2026
* @course CSD1451
* @brief Definition file for a base scene that will be inherited for all scenes
*/

#include "BaseScene.hpp"
#include <algorithm>
#include <iostream>

BaseScene::BaseScene() 
	: scene_entities(0), particleSystem{ new ParticleSystem }, camManager{ CameraManager::GetInstance() } {}

BaseScene::~BaseScene() {
	scene_entities.clear();
	delete particleSystem;
	if (physicsManager)
		delete physicsManager;
	CameraManager::Free();
}

void BaseScene::PreUpdate(const f32& dt) {
	for (auto& entity : scene_entities) {
		entity->PreUpdate(dt);
	}
	if (physicsManager != nullptr) {
		physicsManager->PreUpdate(dt);
	}
}

void BaseScene::Update(const f32& dt) {
	for (auto& entity : scene_entities) {
		entity->Update(dt);
	}
	particleSystem->Update(dt);
	camManager->Update(dt);
	if (physicsManager != nullptr) {
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
	// Sort each entity according to Render Layer, higher Render Layers will be rendered on top
	std::stable_sort(scene_entities.begin(), scene_entities.end(), compare);

	for (auto& entity : scene_entities) {
		entity->Render();
	}
	particleSystem->Render();
	if (physicsManager != nullptr) {
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

void BaseScene::AddEntityToScene(BaseEntity* entity) {
	scene_entities.push_back(entity);
	// Checks and type cast the pointer to game object with entity, if the entity derives from gameobject.
	if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(entity)) {
		if (physicsManager == nullptr) {
			physicsManager = new PhysicsManager{};
		}

		physicsManager->PushGameObject(go);
	}
}

void BaseScene::RemoveEntityFromScene(BaseEntity* entity) {
	bool deleted = false;
	// Remove from scene entities
	for (std::vector<BaseEntity*>::iterator it = scene_entities.begin(); it != scene_entities.end(); it++) {
		if (entity == *it) {
			scene_entities.erase(it);
			deleted = true;
			break;
		}
	}

	// Remove from physics manager
	GameObjectEntity* go;
	if (physicsManager && (go = dynamic_cast<GameObjectEntity*>(entity))) {
		for (std::vector<GameObjectEntity*>::iterator it = physicsManager->gameObjects.begin(); it != physicsManager->gameObjects.end(); it++) {
			if (*it == go) {
				physicsManager->gameObjects.erase(it);
				break;
			}
		}
	}

	// delete memory
	if (deleted) {
		delete entity;
	}
}

std::vector<BaseEntity*> const& BaseScene::Entities() const {
	return scene_entities;
}
