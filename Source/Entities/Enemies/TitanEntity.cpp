#include "TitanEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
TitanEntity::TitanEntity(AEVec2 pos) : ground{nullptr}, EnemyEntity(pos, { 1.f,0.f }) {
	sprite = AssetManager::GetSpriteSheet("Assets/test_troop.png", 3, 3);
	// Empty for now
}

TitanEntity::~TitanEntity() {
	// Empty by design
}

void TitanEntity::PreUpdate(const f32& dt) {
	EnemyEntity::PreUpdate(dt);
}

void TitanEntity::Update(const f32& dt) {
	EnemyEntity::Update(dt);
}

void TitanEntity::PostUpdate(const f32& dt) {
	EnemyEntity::PostUpdate(dt);
}

void TitanEntity::Render() {
	EnemyEntity::Render();
}

void TitanEntity::OnCollide(GameObjectEntity* go) {
	EnemyEntity::OnCollide(go);
	ground = go->go_type == PhysicsType::STATIC ? go : nullptr;
	if (go->go_type == PhysicsType::DYNAMIC) {
		SwitchState(FSM::IDLE, 3.f);
	}
}

void TitanEntity::OnIdle(const f32& dt) {
	// Trooper's idle behaviour
	velocity.x = 0.f;
	if (stateTimer < 0.f) {
		dir.x *= -1.f; // Flip the direction it is travelling.
		SwitchState(FSM::PATROL);
		return;
	}
}

void TitanEntity::OnPatrol(const f32& dt) {
	// Trooper's patrol behaviour
	AEVec2 contactPt, normal;
	f32 timeCollide;
	velocity.x = dir.x * 10.f;
	// Checks if it is on the ledge.
	if (ground != nullptr && !(Utils::RayAABB({ position.x + scale.x * dir.x * 0.5f, position.y }, AEVec2{ 0.f, -1.f }, ground, contactPt, normal, timeCollide)) ) {
		velocity.x = 0.f;
		SwitchState(FSM::IDLE, 3.f); // Switching of states
	}
}

void TitanEntity::OnChase(const f32& dt) {
	// Trooper's chase behaviour
}

void TitanEntity::OnStun(const f32& dt) {
	// Trooper's stun behaviour
}

void TitanEntity::OnDead(const f32& dt) {
	// Trooper's death behaviour
}
