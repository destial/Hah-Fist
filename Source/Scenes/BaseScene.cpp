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
#include "../Managers/SceneManager.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/WeaponEntity.hpp"
#include "../UI/BarUI.hpp"
#include <algorithm>
#include <iostream>

BaseScene::BaseScene() 
: awaiting_deletion(0), scene_entities(0), current_state{FrameState::INIT},
  particleSystem{ new ParticleSystem }, camManager{ CameraManager::GetInstance() } {}

BaseScene::~BaseScene() {
	linked_entities.clear();
	awaiting_deletion.clear();
	for (size_t i{}; i < scene_entities.size(); ++i) {
		BaseEntity* entity = scene_entities[i];
		if (entity) {
			delete entity;
			scene_entities[i] = nullptr;
		}
	}
	scene_entities.clear();
	delete particleSystem;
	if (physicsManager)
		delete physicsManager;
}

void BaseScene::PreUpdate(const f32& dt) {
	current_state = FrameState::PRE;
	awaiting_deletion.clear();
	// Need to use an index-based loop instead of an iterator because entities may be added to scene_entities. 
	// Adding elements to a std::vector can trigger a reallocation, which invalidates all iterators and references. 
	// Using indices avoids iterator invalidation issues during modification.
	for (int i = 0; i < scene_entities.size(); ++i) {
		scene_entities[i]->PreUpdate(dt);
	}
	if (physicsManager != nullptr) {
		physicsManager->PreUpdate(dt);
	}
}

void BaseScene::Update(const f32& dt) {
	current_state = FrameState::MIDDLE;
	// Need to use an index-based loop instead of an iterator because entities may be added to scene_entities. 
	// Adding elements to a std::vector can trigger a reallocation, which invalidates all iterators and references. 
	// Using indices avoids iterator invalidation issues during modification.
	for (int i = 0; i < scene_entities.size(); ++i) {
		scene_entities[i]->Update(dt);
	}
	particleSystem->Update(dt);
	camManager->Update(dt);
}

void BaseScene::PostUpdate(const f32& dt) {
	current_state = FrameState::POST;
	// Need to use an index-based loop instead of an iterator because entities may be added to scene_entities. 
	// Adding elements to a std::vector can trigger a reallocation, which invalidates all iterators and references. 
	// Using indices avoids iterator invalidation issues during modification.
	for (int i = 0; i < scene_entities.size(); ++i) {
		scene_entities[i]->PostUpdate(dt);
	}

	if (physicsManager != nullptr) {
		physicsManager->PostUpdate(dt);
	}

	// Use a seperate loop when deleting to avoid concurrent modification errors
	for (int i = 0; i < awaiting_deletion.size(); ++i) {
		DeleteEntityFromScene(awaiting_deletion[i]);
	}
}

static bool compare(BaseEntity* a, BaseEntity* b) {
	return b->layer > a->layer;
}

void BaseScene::Render() {
	current_state = FrameState::RENDER;
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
	current_state = FrameState::END;
	for (size_t i{}; i < scene_entities.size(); ++i) {
		BaseEntity* entity = scene_entities[i];
		if (entity) {
			delete entity;
			scene_entities[i] = nullptr;
		}
	}
	awaiting_deletion.clear();
	linked_entities.clear();
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

	// Spawn a new healthbar entity and link it to this enemy entity
	if (EnemyEntity* enemy = dynamic_cast<EnemyEntity*>(entity)) {
		BarUI* healthbar = new BarUI{ AEVec2{ 0.f, 0.f } };
		healthbar->scale = { 2.f, .25f };
		healthbar->overlay_color = { 255, 64, 255, 64 };
		healthbar->color = { 255, 255, 64, 64 };
		healthbar->text = "";
		healthbar->layer = BaseUI::RenderLayer::ENTITY;
		healthbar->text_size = 7.f;
		healthbar->SetInteractive(false);
		healthbar->AddPostUpdateListener(this, [healthbar, enemy](const f32&) {
			healthbar->SetValue(enemy->health / enemy->max_health);
			healthbar->position = enemy->position;
			healthbar->position.y += std::abs(enemy->scale.y) * 0.6f;
		});

		AddEntityToScene(healthbar);
		// Link the enemy to this healthbar
		linked_entities[enemy] = healthbar;
	}
}

void BaseScene::RemoveEntityFromScene(BaseEntity* entity) {
	if (dynamic_cast<WeaponEntity*>(entity)) {
		return;
	}
	if (SceneManager::GetInstance()->GetEditor()->IsToggled()) {
		// Level editor is toggled, so BaseScene update functions will not get called
		// so it is safe to immediately delete
		DeleteEntityFromScene(entity);
	}
	else {
		// Use a seperate loop when deleting to avoid concurrent modification errors
		// In the case we remove while inside the update loop
		awaiting_deletion.push_back(entity);
		if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(entity)) {
			go->isActive = false;
		}

		// Special cases with entities linked to other entities (e.g. health bar UI entity)
		if (BaseEntity* linked = linked_entities[entity]) {
			linked_entities.erase(entity);
			awaiting_deletion.push_back(linked);
		}
	}
}

void BaseScene::DeleteEntityFromScene(BaseEntity* entity) {
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
	if (deleted && physicsManager && (go = dynamic_cast<GameObjectEntity*>(entity))) {
		for (std::vector<GameObjectEntity*>::iterator it = physicsManager->gameObjects.begin(); it != physicsManager->gameObjects.end(); it++) {
			if (*it == go) {
				physicsManager->gameObjects.erase(it);
				break;
			}
		}
	}

	// Delete memory
	if (deleted) {
		if (BaseEntity* linked = linked_entities[entity]) {
			linked_entities.erase(entity);
			DeleteEntityFromScene(linked);
		}
		delete entity;
	}
}

std::vector<BaseEntity*> const& BaseScene::Entities() const {
	return scene_entities;
}

FrameState BaseScene::GetFrameState() const {
	return current_state;
}

ParticleSystem* BaseScene::GetParticleSystem() const {
	return particleSystem;
}
