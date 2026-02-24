#include "TrooperEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
TrooperEntity::TrooperEntity(AEVec2 pos) : ground{nullptr}, EnemyEntity(pos, { 1.f,0.f }) {
	sprite = AssetManager::GetSpriteSheet("Assets/test_troop.png", 3, 3);
	// Empty for now
}

TrooperEntity::~TrooperEntity() {
	// Empty by design
}

void TrooperEntity::PreUpdate(const f32& dt) {
	EnemyEntity::PreUpdate(dt);
}

void TrooperEntity::Update(const f32& dt) {
	EnemyEntity::Update(dt);
}

void TrooperEntity::PostUpdate(const f32& dt) {
	EnemyEntity::PostUpdate(dt);
}

void TrooperEntity::Render() {
	EnemyEntity::Render();
}

void TrooperEntity::OnCollide(GameObjectEntity* go) {
	EnemyEntity::OnCollide(go);
	std::cout << "Colliding\n";
	ground = (go->go_type == GameObjectEntity::KINEMATIC::STATIC) ? go : nullptr;
		
}

void TrooperEntity::OnIdle(const f32& dt) {
	// Trooper's idle behaviour
	velocity.x = 0.f;
	if (stateTimer < 0.f) {
		dir.x *= -1.f; // Flip the direction it is travelling.
		SwitchState(FSM::PATROL, 0.f);
		return;
	}
}

void TrooperEntity::OnPatrol(const f32& dt) {
	// Trooper's patrol behaviour
	AEVec2 contactPt, normal;
	f32 timeCollide;
	velocity.x = dir.x * 10.f;
	// Checks if it is on the ledge.
	if (ground != nullptr && !(Utils::RayAABB({ position.x + scale.x * dir.x * 0.5f, position.y }, AEVec2{ 0.f, -1.f }, ground, contactPt, normal, timeCollide)) ) {
		std::cout << "Off platform\n";
		velocity.x = 0.f;
		SwitchState(FSM::IDLE, 3.f); // Switching of states
	}
}

void TrooperEntity::OnChase(const f32& dt) {
	// Trooper's chase behaviour
}

void TrooperEntity::OnStun(const f32& dt) {
	// Trooper's stun behaviour
}

void TrooperEntity::OnDead(const f32& dt) {
	// Trooper's death behaviour
}
