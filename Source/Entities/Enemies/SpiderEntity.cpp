#include "SpiderEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../StaticEntity.hpp"

SpiderEntity::SpiderEntity(AEVec2 pos, f32 speed, bool spawnHatchlings) : bSpawnHatchlings{spawnHatchlings}, EnemyEntity(pos, {1.f,0.f}, speed, true) {
	InitializeAnimatedSpriteData(ASSET_SPIDER_SPRITE, ASSET_SPIDER_SPRITE_ROWS, ASSET_SPIDER_SPRITE_COLUMNS, ASSET_SPIDER_SPRITE_SCALE);
}

SpiderEntity::~SpiderEntity() {} // Empty dtor

void SpiderEntity::PostUpdate(const f32& dt) {
	// Animation
	currentRow = 0;
	if (velocity.x > 0) {
		currentRow = 2;
	}
	if (velocity.x < 0) {
		currentRow = 1;
	}/*
	if (death) {
		currentRow = 3
	}*/
	GameObjectEntity::PostUpdate(dt);
}

void SpiderEntity::OnCollide(GameObjectEntity* go) {
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

void SpiderEntity::OnHit() {
	AEAudioPlay(AssetManager::GetAudio(ASSET_SPIDERHURT_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
}

void SpiderEntity::OnIdle(const f32&) {
	// Spider's idle behaviour
	velocity.x = 0.f;

	if (stateTimer < 0.f) {
		dir.x *= -1.f; // Flip the direction it is travelling.
		SwitchState(FSM::PATROL);
		return;
	}
}

void SpiderEntity::OnDead() {
	// Checks if this spider can spawn hatchlings
	if (bSpawnHatchlings) {
		SpiderEntity* baby = new SpiderEntity(position, 15.f, false);
		f32 dr = rand() % 2 ? -1.f : 1;
		baby->velocity = { AERandFloat() * -5.f, AERandFloat() * 5.f };
		baby->SwitchState(FSM::IDLE);
		baby->scale = { scale.x * 0.5f * dr, scale.y * 0.5f };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(baby);
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
