/*!
* @file PayloadEntity.cpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 8 March 2026
* @course CSD1451
* @brief Implementation of the PayloadEntity boss class. This class defines
*        behavior for a boss enemy that uses a multi-phase attack system,
*        including jumping, platform spawning, and projectile attacks.
*        The boss scales its attack intensity based on remaining health.
*/
#include "PayloadEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/ExplosiveProjectile.hpp"
#include "../Projectiles/MissileProjectile.hpp"
#include "../StaticEntities/MovingPlatformEntity.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../../Scenes/GameScene.hpp"
/*!
* @brief Constructs the PayloadEntity and initializes its attributes such as
*        sprite data, attack parameters, and internal state machine
* @param pos - Initial position of the Payload boss
* @return None
*/
PayloadEntity::PayloadEntity(AEVec2 pos) : ground{nullptr}, BossEntity(pos) {
	//Initialising Sprite Data and Boss Variables
	InitializeAnimatedSpriteData(ASSET_PAYLOAD_SPRITE, ASSET_PAYLOAD_SPRITE_ROWS, ASSET_PAYLOAD_SPRITE_COLUMNS, ASSET_PAYLOAD_SPRITE_SCALE);
	land_timer = 0.f;
	inner_state = INNERFSM::JUMP;
	frictionMultiplier = BOSS2FRICTION;
}
/*!
* @brief Destructor for PayloadEntity
* @return None
*/
PayloadEntity::~PayloadEntity() {
	// Empty body
}
/*!
* @brief Handles post-update logic such as animation selection and sprite orientation
* @param dt - Delta time since last frame
* @return None
*/
void PayloadEntity::PostUpdate(const f32& dt) {
	//Changes animation of spritesheet
	current_row = 1;
	if (fabsf(velocity.x) > 0.1f) {
		current_row = 0;
		if (velocity.x < 0)
		{
			if (this->scale.x > 0)
			{
				this->scale.x *= -1;
			}
		}
		else
		{
			if (this->scale.x < 0)
			{
				this->scale.x *= -1;
			}
		}
		if (AEVec2Length(&velocity) > 50.0f)
		{
			current_row = 2;
		}
	}
	BossEntity::PostUpdate(dt);
}
/*!
* @brief Handles idle state behavior and transitions to chase when activated
* @param dt - Delta time since last frame (unused)
* @return None
*/
void PayloadEntity::OnIdle(const f32&) {
	//Activate the Boss Fight
	if (boss_activated) {
		SwitchState(FSM::CHASE);
	}
}
/*!
* @brief Handles chase behavior using an internal state machine for jump,
*        landing, and attack phases
* @param dt - Delta time since last frame
* @return None
*/
void PayloadEntity::OnChase(const f32& dt) {
	//Used an INNERFSM to help with more varied options for the boss
	switch (inner_state) {
	case INNERFSM::JUMP:
	{
		//Randomly Chooses a direction to jump towards
		dir.x = (Utils::RandRange(0.f, 1.f) < 0.5f) ? -1.f : 1.f;
		velocity.x = dir.x * BOSS2JUMPVELX;
		//The boss will jump higher when he near the ground and will jump lower when he is higher to the room
		//This is to make the boss fight feel like he is generally above you without him jumping out of the screen
		float t = AEClamp(position.y / BOSS2ROOMMAXHEIGHT, 0.f, 1.f);
		velocity.y = BOSS2JUMPVELY * (1.f - t * t);
		inner_state = INNERFSM::LAND;
		land_timer = land_cooldown;
		break;
	}
	case INNERFSM::LAND:
	{
		//Use a timer to spawn a platform below the boss after he jumps
		land_timer -= dt;
		if (land_timer < 0.f)
		{
			//Spawns the platform below the boss for him to land on
			AEVec2 Pos{ position.x,  position.y - scale.y * 0.7f };
			//Direction that the platform will move in which is downwards
			AEVec2 platformDir{ 0.f, -1.f };
			//Standard Platform Spawning
			MovingPlatformEntity* platform = new MovingPlatformEntity(Pos, platformDir, false , 0.2f , 10.0f);
			platform->mesh = MeshRenderer::GetCenterRectMesh();
			platform->scale = { 7.f, 0.5f };
			SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(platform);
			inner_state = INNERFSM::ATTACK;
		}
		break;
	}
	case INNERFSM::ATTACK:
	{
		//Add projectiles based on the health of the boss
		int projectiles = BOSS2BASEPROJECTILES + static_cast<int>(GetLowHealthFactor() * BOSS2EXTRAPROJECTILES);
		for (int i = 0; i < projectiles; i++)
		{
			//Spawn the projectiles based on the attack range of the boss
			AEVec2 Pos{ Utils::RandRange(position.x - BOSS2ATTACKRANGE,position.x + BOSS2ATTACKRANGE),  position.y };
			//Shoots downwards from the boss
			AEVec2 shootDir{ 0.f, -1.f };
			ShootProjectile(health / max_health, Pos, shootDir);
		}
		inner_state = INNERFSM::JUMP;
		//Boss is stunned less as he gets lower health
		float stunTime = 2.f - (1.f * GetLowHealthFactor());
		SwitchState(FSM::STUN, stunTime);
		break;
	}
	default:
		break;
	}
}
/*!
* @brief Handles stun state behavior and transitions back to chase when timer expires
* @param dt - Delta time since last frame (unused)
* @return None
*/
void PayloadEntity::OnStun(const f32&) {
	if (state_timer < 0.f) {
		SwitchState(FSM::CHASE);
	}
}
/*!
* @brief Handles boss death by triggering win condition and removing the entity
* @return None
*/
void PayloadEntity::OnDead() {
	if (GameScene* game = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetCurrentScene()))
	{
		game->Win();
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);

}