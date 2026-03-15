#include "TrooperEntity.hpp"
#include "../StaticEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
TrooperEntity::TrooperEntity(AEVec2 pos, f32 speed) : EnemyEntity(pos, { 1.f,0.f }, speed, true) {
	InitializeAnimatedSpriteData(ASSET_SLIMETROOP_SPRITE, ASSET_SLIMETROOP_SPRITE_ROWS, ASSET_SLIMETROOP_SPRITE_COLUMNS, ASSET_TROOPER_SPRITE_SCALE);
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
	// Animation of Trooper Entity
	if (velocity.x < 0)
	{
		if (this->scale.x < 0)
		{
			this->scale.x *= -1;
		}
	}
	else
	{
		if (this->scale.x > 0)
		{
			this->scale.x *= -1;
		}
	}
	GameObjectEntity::PostUpdate(dt);
}

void TrooperEntity::Render() {
	EnemyEntity::Render();
}

void TrooperEntity::OnCollide(GameObjectEntity* go) {
	EnemyEntity::OnCollide(go);
	if (go->go_type == PhysicsType::DYNAMIC) {
		if (EnemyEntity* e = dynamic_cast<EnemyEntity*>(go)) {
			e->FlipDir();
			return;
		}
		SwitchState(FSM::IDLE, 2.f);
	} 
	else if (go->go_type == PhysicsType::STATIC) {
		if (StaticEntity* se = dynamic_cast<StaticEntity*>(go)) {
			if (se->GetStaticType() == StaticEntity::STATIC_TYPE::TYPE_WALL) {
				if (position.y - scale.y * 0.5f < go->position.y + go->scale.y * 0.5f) {
					SwitchState(FSM::IDLE, 2.f);
				}
			}
		}
	}
}

void TrooperEntity::OnHit()
{
	EnemyEntity::OnHit();
}

void TrooperEntity::OnIdle(const f32& dt) {
	// Trooper's idle behaviour
	velocity.x = 0.f;
	if (stateTimer < 0.f) {
		FlipDir(); // Flip the direction it is travelling.
		SwitchState(FSM::PATROL);
		return;
	}
}

void TrooperEntity::OnPatrol(const f32& dt) {
	EnemyEntity::OnPatrol(dt);
}

void TrooperEntity::OnChase(const f32& dt) {
	// Trooper's chase behaviour
}

void TrooperEntity::OnStun(const f32& dt) {
	// Trooper's stun behaviour
}

void TrooperEntity::OnDead(const f32& dt) {
	// Trooper's death behaviour
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
