/*!
* @file DropEntity.cpp
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @date 12th March 2026
* @course CSD1451
* @brief This source file defines the DropEntity class' member functions, which
* are pickups for the player.
*/
#include "DropEntity.hpp"
#include "../Managers/SceneManager.hpp"

DropEntity::DropEntity(AEVec2 pos, f32 mass, CollisionShape shape) : GameObjectEntity(pos, mass, shape, PhysicsType::TRIGGER) {}

DropEntity::~DropEntity() {}

void DropEntity::OnCollide(GameObjectEntity* go) {
	if (go->entity_type == EntityType::PLAYER) {
		OnPickup(go);
		SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
	}
}

void DropEntity::Update(const f32&) {
	// Empty body
}

void DropEntity::OnPickup(GameObjectEntity*) {
	// Empty body
}