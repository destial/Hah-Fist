/*!
* @file TitanEntity.cpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 8 March 2026
* @course CSD1451
* @brief Implementation of the TitanEntity boss class. This class defines
*        the behavior of a boss enemy including movement, state transitions,
*        attack patterns, projectile spawning, and death handling.
*/
#include "TitanEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/SpikeProjectile.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../../Scenes/GameScene.hpp"
#include "../PlayerEntity.hpp"
/*!
* @brief Constructs the TitanEntity and initializes its attributes such as
*        sprite data, attack parameters, and projectile configuration
* @param pos - Initial position of the Titan boss
* @return None
*/
TitanEntity::TitanEntity(AEVec2 pos) : ground{nullptr}, BossEntity(pos) {
	//Initialising Sprite Data and Boss Variables
	InitializeAnimatedSpriteData(ASSET_TITAN_SPRITE, ASSET_TITAN_SPRITE_ROWS, ASSET_TITAN_SPRITE_COLUMNS, ASSET_TITAN_SPRITE_SCALE);
	shoot_timer = 0.f;
}
/*!
* @brief Destructor for TitanEntity
* @return None
*/
TitanEntity::~TitanEntity() {
	// Empty body
}
/*!
* @brief Updates the Titan boss logic including cooldown timers and base behavior
* @param dt - Delta time since last frame
* @return None
*/
void TitanEntity::Update(const f32& dt) {
	//Update Shootimer based on dt
	shoot_timer -= dt;
	BossEntity::Update(dt);
}
/*!
* @brief Handles post-update logic such as animation state selection and sprite flipping
* @param dt - Delta time since last frame
* @return None
*/
void TitanEntity::PostUpdate(const f32& dt) {
	//Changes animation of spritesheet if the boss is jumping and flips the sprite when it is looking left or right
	currentRow = 1;
	if (velocity.x != 0) {
		currentRow = 0;
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
	}
	if (velocity.y != 0.f)
	{
		currentRow = 2;
	}
	BossEntity::PostUpdate(dt);
}
/*!
* @brief Handles idle state behavior and transitions to chase when activated
* @param dt - Delta time since last frame (unused)
* @return None
*/
void TitanEntity::OnIdle(const f32&) {	
	//Activate the Boss Fight
	if (boss_activated) {
		SwitchState(FSM::CHASE);
	}
}
/*!
* @brief Handles patrol state behavior and transitions to chase after timer expires
* @param dt - Delta time since last frame (unused)
* @return None
*/
void TitanEntity::OnPatrol(const f32&) {
	if (stateTimer < 0.f) {
		SwitchState(FSM::CHASE);
	}
}
/*!
* @brief Handles chase behavior by jumping toward the player and transitioning to stun
* @param dt - Delta time since last frame (unused)
* @return None
*/
void TitanEntity::OnChase(const f32&) {
	//Checks for the player
	PlayerEntity* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<PlayerEntity>();
	if (!player) return;
	//If able to find the player then try to jump towards their position
	dir.x = (player->position.x > position.x) ? 1.f : -1.f;
	//Boss jumps further based on how low he is up to twice as far
	velocity.x = dir.x * (BOSS1JUMPVELX + GetLowHealthFactor() * BOSS1JUMPVELX);
	velocity.y = BOSS1JUMPVELY;
	
	SwitchState(FSM::STUN, 3.f);


}
/*!
* @brief Handles stun state behavior including spawning projectiles and camera shake
* @param dt - Delta time since last frame (unused)
* @return None
*/
void TitanEntity::OnStun(const f32&) {
	//Check if the boss is on the ground
	if (pBody->is_standing_above && shoot_timer < 0.f) {
		//Screenshake to give the player feedback the boss has landed and will start attacking the player with the spike projectiles
		CameraManager::GetInstance()->Shake(3.f, 5.f);
		//Add projectiles based on the health of the boss
		int projectiles = BOSS1BASEPROJECTILES + static_cast<int>(GetLowHealthFactor() * BOSS1EXTRAPROJECTILES);
		for (int i = 0; i < projectiles; i++) {
			//Spawn the spike projectiles based on the boss room Attackrange being the half size of the room so it spawns 
			//spikes from the ceiling of the bossroom
			AEVec2 Pos{ Utils::RandRange(boss_room_center.x - BOSS1ATTACKRANGE,boss_room_center.x + BOSS1ATTACKRANGE), BOSS1ROOMPOSY };
			//Shoots downwards from the ceiling
			AEVec2 shootDir{ 0.f, -1.f };
			//Normalizing shoot direction
			AEVec2Normalize(&shootDir, &shootDir);
			//Random Projectile Speed
			f32 bulletSpeed = Utils::RandRange(BULLETMINSPEED, BULLETMAXSPEED);
			//Normal Spike Spawn Code
			SpikeProjectile* spike = new SpikeProjectile(Pos, shootDir, bulletSpeed, this->damage, this);
			spike->scale = { BULLETSCALEX ,BULLETSCALEY };
			SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(spike);
		}
		//Reset Shoot_timer so boss does not constantly shoot
		shoot_timer = shoot_cooldown;
	}

	//Reset back to patrol state
	if (stateTimer < 0.f) {
		SwitchState(FSM::PATROL, 2.f);
	}
}
/*!
* @brief Handles boss death by triggering win condition and removing the entity
* @return None
*/
void TitanEntity::OnDead() {
	//Set winscreen to popup
	if (GameScene* game = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetCurrentScene())) {
		game->Win();
	}
	//Delete entity from the scene
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
