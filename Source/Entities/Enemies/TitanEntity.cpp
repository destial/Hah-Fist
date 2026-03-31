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
	InitializeAnimatedSpriteData(ASSET_TITAN_SPRITE, ASSET_TITAN_SPRITE_ROWS, ASSET_TITAN_SPRITE_COLUMNS, ASSET_TITAN_SPRITE_SCALE);
	attack_range = BOSS1ATTACKRANGE;
	shoot_timer = 0.f;
	jump_x = BOSS1JUMPVELX;
	jump_y = BOSS1JUMPVELY;
	base_projectiles = BOSS1BASEPROJECTILES;
	extra_projectiles = BOSS1EXTRAPROJECTILES;
	
}
/*!
* @brief Destructor for TitanEntity
* @return None
*/
TitanEntity::~TitanEntity() {
}
/*!
* @brief Updates the Titan boss logic including cooldown timers and base behavior
* @param dt - Delta time since last frame
* @return None
*/
void TitanEntity::Update(const f32& dt) {
	shoot_timer -= dt;
	BossEntity::Update(dt);
}
/*!
* @brief Handles post-update logic such as animation state selection and sprite flipping
* @param dt - Delta time since last frame
* @return None
*/
void TitanEntity::PostUpdate(const f32& dt) {
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
	PlayerEntity* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<PlayerEntity>();
	if (!player) return;

	dir.x = (player->position.x > position.x) ? 1.f : -1.f;
	float healthRatio = health / max_health;
	float temp = (1.f - healthRatio) / 0.75f;
	temp = AEClamp(temp, 0.f, 1.f);
	velocity.x = dir.x * (jump_x + temp * jump_x);
	velocity.y = jump_y;
	
	SwitchState(FSM::STUN, 3.f);


}
/*!
* @brief Handles stun state behavior including spawning projectiles and camera shake
* @param dt - Delta time since last frame (unused)
* @return None
*/
void TitanEntity::OnStun(const f32&) {
	if (pBody->is_standing_above && shoot_timer < 0.f) {
		CameraManager::GetInstance()->Shake(3.f, 5.f);
		float healthRatio = health / max_health;
		float temp = (1.f - healthRatio) / 0.75f;
		temp = AEClamp(temp, 0.f, 1.f);

		int projectiles = base_projectiles + static_cast<int>(temp * extra_projectiles);
		for (int i = 0; i < projectiles; i++) {
			AEVec2 Pos{ Utils::RandRange(boss_room_center.x-attack_range,boss_room_center.x +attack_range), BOSS1ROOMPOSY };
			AEVec2 shootDir{ 0.f, -1.f };
			AEVec2Normalize(&shootDir, &shootDir);
			f32 bulletSpeed = Utils::RandRange(BULLETMINSPEED, BULLETMAXSPEED);

			SpikeProjectile* spike = new SpikeProjectile(Pos, shootDir, bulletSpeed, this->damage, this);
			spike->scale = { BULLETSCALEX ,BULLETSCALEY };
			SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(spike);
		}

		shoot_timer = shoot_cooldown;
	}


	if (stateTimer < 0.f) {
		SwitchState(FSM::PATROL, 2.f);
	}
}
/*!
* @brief Handles boss death by triggering win condition and removing the entity
* @return None
*/
void TitanEntity::OnDead() {
	if (GameScene* game = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetCurrentScene())) {
		game->Win();
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
