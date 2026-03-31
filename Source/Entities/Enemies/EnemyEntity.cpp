/*!
* @file EnemyEntity.hpp
* @author Name (brandonshaohui.koh@digipen.edu)
* @date 21 January 2026
* @course CSD1451
* @brief Declaration/Definition file for abcxyz
*/
#include "EnemyEntity.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Utils/Constant.hpp"
#include "../DropEntities/CoinEntity.hpp"
#include "../../Scenes/GameScene.hpp"

/*!
* @brief Constructor of the EnemyEntity class which initialises the position, direction, speed.
* @param pos - Position of the EnemyEntity
* @param dir - Direction of the EnemyEntity
* @param speed - Speed of the EnemyEntity
* @param inherited - Status of EnemyEntity's inheritance
*/
EnemyEntity::EnemyEntity(AEVec2 pos, AEVec2 dir, f32 speed, bool inherited) 
: GameObjectEntity{ pos }, state{ FSM::IDLE }, dir{ dir }, speed{ speed }, stateTimer{ 1.f }
{
	//Behaviour that would be called again by the inherited class, to prevent the extra load, this only runs in the base class
	if (!inherited)
	{
		InitializeAnimatedSpriteData(ASSET_BASEENEMY_SPRITE, ASSET_BASEENEMY_SPRITE_ROWS, ASSET_BASEENEMY_SPRITE_COLUMNS, ASSET_BASEENEMY_SPRITE_SCALE);
	}
	layer = RenderLayer::ENTITY;
	entity_type = EntityType::ENEMY;
	health = DEFAULTENEMYMAXHEALTH;
	max_health = DEFAULTENEMYMAXHEALTH;
	damage = DEFAULTENEMYDAMAGE;

}

/*!
* @brief PreUpdate function
* @param dt - Time between each frame
*/
void EnemyEntity::PreUpdate(const f32& dt)
{
	GameObjectEntity::PreUpdate(dt);
}


/*!
* @brief Update function
* @param dt - Time between each frame
*/
void EnemyEntity::Update(const f32& dt)
{
	GameObjectEntity::Update(dt);
	if (health <= 0.f && state != FSM::DEAD) {
		SwitchState(FSM::DEAD, 3.f);
	}
	stateTimer -= dt;
	switch (state) {
		case FSM::IDLE: 
		{
			OnIdle(dt);
			break;
		}
		case FSM::PATROL:
		{
			OnPatrol(dt);
			break;
		}
		case FSM::CHASE:
		{
			OnChase(dt);
			break;
		}
		case FSM::ATTACK:
		{
			OnAttack(dt);
			break;
		}
		case FSM::STUN:
		{
			OnStun(dt);
			break;
		}
		case FSM::DEAD:
		{
			OnDead();
			break;
		}
		default:
			break;
	}

	this->color = Utils::Lerp(Color{ 255, 255, 128, 128 }, Color{ 255, 255, 255, 255 }, timeElapsedSinceLastDamage / PLAYER_CONTROL_LOCK_AFTER_HIT);
}

/*!
* @brief PostUpdate function
* @param dt - Time between each frame
*/
void EnemyEntity::PostUpdate(const f32& dt)
{
	currentRow = 0;
	if (velocity.x > 0) {
		currentRow = 1;
	}
	if (velocity.x < 0) {
		currentRow = 2;
	}
	GameObjectEntity::PostUpdate(dt);
}

/*!
* @brief OnHit function. This contains the collision response of the EnemyEntity
*/
void EnemyEntity::OnHit()
{
	timeElapsedSinceLastDamage = 0.0f;
	//invulnerabilityDuration = 0.75f;
	AEAudioPlay(AssetManager::GetAudio(ASSET_ENEMYHURT_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);

	SceneManager::GetInstance()->GetCurrentScene()->GetParticleSystem()->SpawnParticles(ParticleType::SPARKLE, this->position, 3, 1.f, 1.f, 0.5f);
}

/*!
* @brief OnIdle function. This contains the idle behaviour of the EnemyEntity
*/
void EnemyEntity::OnIdle(const f32&)
{
	// Empty by design
}

/*!
* @brief OnPatrol function. This contains the Patrol behaviour of the EnemyEntity
*/
void EnemyEntity::OnPatrol(const f32&) {
	// Clamps the acceleration of object
	if (abs(velocity.x) < speed)
	{
		velocity.x += dir.x * speed;
	}
	// Checks if it is on the ledge.
	if (!Utils::RayHitAny({ position.x + std::abs(scale.x) * dir.x * 0.5f, position.y - std::abs(scale.y) * 0.5f }, AEVec2{ 0.f, -1.f }, GameScene::GetStaticEntities())) {
		velocity.x = 0.f;
		SwitchState(FSM::IDLE, 3.f);
	}
}

/*!
* @brief OnChase function. This contains the Chasing behaviour of the EnemyEntity
*/
void EnemyEntity::OnChase(const f32&) {
	// Empty by design
}

/*!
* @brief OnAttack function. This contains the Attack behaviour of the EnemyEntity
*/
void EnemyEntity::OnAttack(const f32&) {
	// Empty by design
}

/*!
* @brief OnStun function. This contains the Stun behaviour of the EnemyEntity
*/
void EnemyEntity::OnStun(const f32&) {
	// Empty by design
}

/*!
* @brief OnDead function. This contains the Dead behaviour of the EnemyEntity
*/
void EnemyEntity::OnDead()
{
	CoinEntity* coin = new CoinEntity{ position };
	SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(coin);
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}

/*!
* @brief Helper function for switching the FSM states of the Enemy
*/
void EnemyEntity::SwitchState(FSM newState, f32 timeInNewState)
{
	state = newState;
	stateTimer = timeInNewState;
}

/*!
* @brief Fetches the current state of the enemy.
* @return FSM
*/
EnemyEntity::FSM EnemyEntity::GetCurrentState() const
{
	return state;
}

/*!
* @brief Helper function for switching the direction of the enemy.
*/
void EnemyEntity::FlipDir()
{
	dir.x *= -1.f;
}
