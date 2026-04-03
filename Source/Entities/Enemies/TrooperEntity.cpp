/*!
* @file TrooperEntity.cpp
* @author Name (brandonshaohui.koh@digipen.edu)
* @date 24 February 2026
* @course CSD1451
* @brief This source file contians the definitions of class TrooperEntity as well as its member functions
*		 which is the slime enemy in the game.
*/
#include "TrooperEntity.hpp"
#include "../StaticEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#define COLLISION_OFFSET 1.f

/*!
* @brief Constructor of the TrooperEntity class. Takes in the position & speed and intialises it.
* @param pos - Position of the trooper
* @param speed - Speed of the trooper
*/
TrooperEntity::TrooperEntity(AEVec2 pos, f32 speed) : EnemyEntity(pos, { 1.f,0.f }, speed, true) {
	InitializeAnimatedSpriteData(ASSET_SLIMETROOP_SPRITE, ASSET_SLIMETROOP_SPRITE_ROWS, ASSET_SLIMETROOP_SPRITE_COLUMNS, ASSET_TROOPER_SPRITE_SCALE);
}

/*!
* @brief Destructor of the Trooper Entity
*/
TrooperEntity::~TrooperEntity() {
	// Empty by design
}

/*!
* @brief PostUpdate of TrooperEntity
*/
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

/*!
* @brief OnCollide function which contains the collision response of the TrooperEntity
*/
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
				if (position.y - scale.y * 0.5f + COLLISION_OFFSET < go->position.y + go->scale.y * 0.5f) {
					SwitchState(FSM::IDLE, 2.f);
				}
			}
		}
	}
}

/*!
* @brief OnIdle override of EnemyEntity class. This contains the Idle behaviour of the Trooper(Slime)
* @param dt - Time between every frame
*/
void TrooperEntity::OnIdle(const f32&) {
	// Trooper's idle behaviour
	velocity.x = 0.f;
	if (state_timer < 0.f) {
		FlipDir(); // Flip the direction it is travelling.
		SwitchState(FSM::PATROL);
		return;
	}
}

/*!
* @brief OnDead override of EnemyEntity class. This contains the Dead behaviour of the Trooper(Slime) when it dies.
*/
void TrooperEntity::OnDead() {
	// Trooper's death behaviour
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
