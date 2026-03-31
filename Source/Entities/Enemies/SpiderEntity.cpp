/*!
* @file SpiderEntity.cpp
* @author Name (brandonshaohui.koh@digipen.edu)
* @date 8 March 2026
* @course CSD1451
* @brief This source file contains the definitions of class SpiderEntity as well as its member functions
*		 including the behaviours.
*/
#include "SpiderEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../StaticEntity.hpp"

/*!
* @brief SpiderEntity constructor
* @param pos - Position of the SpiderEntity
* @param speed - Speed of the SpiderEntity
* @param spawn_hatchlings - If true SpiderEntity will spawn tiny spiders
*/
SpiderEntity::SpiderEntity(AEVec2 pos, f32 speed, bool spawnHatchlings) : spawn_hatchlings{spawnHatchlings}, EnemyEntity(pos, {1.f,0.f}, speed, true) {
	InitializeAnimatedSpriteData(ASSET_SPIDER_SPRITE, ASSET_SPIDER_SPRITE_ROWS, ASSET_SPIDER_SPRITE_COLUMNS, ASSET_SPIDER_SPRITE_SCALE);
}

/*!
* @brief Destructor of SpiderEntity
*/
SpiderEntity::~SpiderEntity() {} // Empty dtor

/*!
* @brief PostUpdate function of the SpiderEntity
* @param dt - Time between every frame
*/
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

/*!
* @brief OnCollide function which takes care of the collision response of the SpiderEntity
* @param go - GameObjectEntity it is colliding with
*/
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

/*!
* @brief OnHit function which takes care of the collision effects
*/
void SpiderEntity::OnHit() {
	AEAudioPlay(AssetManager::GetAudio(ASSET_SPIDERHURT_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
}

/*!
* @brief OnIdle function which takes care of the SpiderEnttiy idle behaviour
* @param dt - Time between every frame
*/
void SpiderEntity::OnIdle(const f32&) {
	// Spider's idle behaviour
	velocity.x = 0.f;

	if (stateTimer < 0.f) {
		dir.x *= -1.f; // Flip the direction it is travelling.
		SwitchState(FSM::PATROL);
		return;
	}
}

/*!
* @brief OnDead function which takes care of the Dead behaviour of the SpiderEntity
*/
void SpiderEntity::OnDead() {
	// Checks if this spider can spawn hatchlings
	if (spawn_hatchlings) {
		SpiderEntity* baby = new SpiderEntity(position, 15.f, false);
		f32 dr = rand() % 2 ? -1.f : 1;
		baby->velocity = { AERandFloat() * -5.f, AERandFloat() * 5.f };
		baby->SwitchState(FSM::IDLE);
		baby->scale = { scale.x * 0.5f * dr, scale.y * 0.5f };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(baby);
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
