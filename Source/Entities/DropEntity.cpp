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