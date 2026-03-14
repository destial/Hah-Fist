#include "TrooperEntity.hpp"
#include "../StaticEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../../Scenes/GameScene.hpp"
TrooperEntity::TrooperEntity(AEVec2 pos, f32 speed) : EnemyEntity(pos, { 1.f,0.f }, speed) {
	sprite = AssetManager::GetSpriteSheet(ASSET_SLIMETROOP_SPRITE, 2, 3);
	animationFrame = 1.f / (2.f * 3.f);
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
	GameObjectEntity::PostUpdate(dt);
	// Animation of Trooper Entity
	if (velocity.x > 0) {
		currentRow = 0;
	}
	if (velocity.x < 0) {
		currentRow = 1;
	}
	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (++currentCol >= 3) {
			currentCol = 0;
		}
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
